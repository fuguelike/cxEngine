//
//  cxUtil.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <zlib.h>
#include <evhttp.h>
#include <sys/time.h>
#include <ctype.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <core/cxEngine.h>
#include "cxBase.h"
#include "cxUtil.h"

cxString cxLuaToString(lua_State *L)
{
    int nargs = lua_gettop(L);
    cxString log = CX_CREATE(cxString);
    for (int i=1; i <= nargs; i++){
        if(lua_isstring(L, i)){
            cxStringFormat(log, "%s",lua_tostring(L, i));
        }else if(lua_isnumber(L, i)){
            cxStringFormat(log, "%f",lua_tonumber(L, i));
        }
    }
    return log;
}

static cxInt cxLuaLogger(lua_State *L)
{
    cxString log = cxLuaToString(L);
    CX_LOGGER("%s",cxStringBody(log));
    CX_UNUSED_PARAM(log);
    return 0;
}

static cxInt cxLuaError(lua_State *L)
{
    cxString log = cxLuaToString(L);
    CX_ERROR("%s",cxStringBody(log));
    return 0;
}

static cxInt cxLuaWarn(lua_State *L)
{
    cxString log = cxLuaToString(L);
    CX_WARN("%s",cxStringBody(log));
    return 0;
}

static cxInt cxLuaAssert(lua_State *L)
{
    cxBool cond = lua_toboolean(L, 1);
    cxConstChars msg = lua_tostring(L, 2);
    CX_ASSERT(cond, msg);
    return 0;
}

static cxInt cxLuaMD5(lua_State *L)
{
    cxSize l = 0;
    cxConstChars s = luaL_checklstring(L, 1, &l);
    cxString data = CX_CREATE(cxString);
    cxStringAppend(data, s, l);
    cxString md5 = cxMD5(data);
    lua_pushstring(L, cxStringBody(md5));
    return 1;
}

const luaL_Reg global_functions [] = {
    {"cxMD5",cxLuaMD5},
    {"cxLogger", cxLuaLogger},
    {"cxError", cxLuaError},
    {"cxWarn", cxLuaWarn},
    {"cxAssert", cxLuaAssert},
    {NULL, NULL}
};

void cxUtilTypeInit()
{
    lua_getglobal(gL, "_G");
    luaL_register(gL, "_G", global_functions);
    lua_pop(gL, 2);
}

cxBool cxCopyFile(cxConstChars file,cxCopyFileFunc func,cxAny udata)
{
    cxStream src = cxAssetsStreamCreate(file);
    cxStream dst = cxFileStreamCreate(file);
    if(!cxStreamOpen(src)){
        return false;
    }
    if(!cxStreamOpen(dst)){
        return false;
    }
    cxChar buffer[4096];
    cxProgress p = {0};
    p.current = 0;
    p.total = src->length;
    while (true) {
        cxInt rbytes = cxStreamRead(src, buffer, 4096);
        if(rbytes <= 0){
            break;
        }
        p.current += rbytes;
        cxInt wbytes = cxStreamWrite(dst, buffer, rbytes);
        CX_ASSERT(wbytes == rbytes, "rw error");
        if(func != NULL){
            func(file,&p,udata);
        }
        CX_UNUSED_PARAM(wbytes);
    }
    return true;
}

cxUInt cxBinaryToUInt(const cxChar *bs)
{
    CX_ASSERT(bs != NULL, "args error");
    cxInt len = strlen(bs);
    cxUInt ret = 0;
    for(cxInt i=0; i < len; i++){
        CX_ASSERT(bs[i] == '0' || bs[i] == '1', "str error");
        if(bs[i] != '1'){
            continue;
        }
        ret |= (1 << (len - i - 1));
    }
    return ret;
}

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c)
{
    for(cxInt i=0; i < strlen(sp);i++){
        if(sp[i] == c){
            return true;
        }
    }
    return  false;
}

cxUInt cxHexToUInt(const cxChar *bs)
{
    CX_ASSERT(bs != NULL, "args error");
    cxInt len = strlen(bs);
    cxUInt ret = 0;
    for(cxInt i=0; i < len; i++){
        cxChar v = tolower(bs[i]);
        cxUInt tmp = 0;
        if(v >= '0' && v <= '9'){
            tmp = v - '0';
        }else if(v >= 'a' && v <= 'f'){
            tmp = v - 'a' + 0xa;
        }else{
            CX_ASSERT(false, "str error");
        }
        ret |= (tmp << ((len - i - 1)) * 4);
    }
    return ret;
}

cxAudioFileType cxAudioGetType(cxConstChars file)
{
    cxConstChars ext = strrchr(file, '.');
    CX_RETURN(ext == NULL,cxAudioFileTypeNone);
    if(cxConstCharsEqu(ext, ".mp3")){
        return cxAudioFileTypeMP3;
    }
    if(cxConstCharsEqu(ext, ".wav")){
        return cxAudioFileTypeWAV;
    }
    return cxAudioFileTypeNone;
}

cxString cxCompressed(cxString data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStringLength(data);
    uLongf size = compressBound(datasiz);
    char *cv = allocator->malloc(size);
    if(compress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), datasiz) == Z_OK){
        return cxStringAttach(cv, size);
    }
    allocator->free(cv);
    return NULL;
}

cxString cxDecompress(cxString data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStringLength(data);
    uLongf size = datasiz * 4;
    char *cv = allocator->malloc(size);
    int ret = uncompress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), (uLongf)datasiz);
    while(ret == Z_BUF_ERROR){
        size = (size << 1);
        cv = allocator->realloc(cv,size);
        ret = uncompress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), (uLongf)datasiz);
    }
    if(ret == Z_OK){
        return cxStringAttach(cv, size);
    }
    allocator->free(cv);
    return NULL;
}

cxInt cxRand(cxInt min,cxInt max)
{
    cxInt x = rand();
    return (min + x % (max + 1 - min));
}

void cxSetRandSeed()
{
    srand((unsigned)clock());
}

cxDouble cxTimestamp()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}

void cxUtilInfo(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("INFO", file, line, format, ap);
    va_end(ap);
}

void cxUtilError(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("ERROR", file, line, format, ap);
    va_end(ap);
}

void cxUtilWarn(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("WARN", file, line, format, ap);
    va_end(ap);
}

void cxUtilAssert(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("ASSERT", file, line, format, ap);
    va_end(ap);
}