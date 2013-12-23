//
//  cxString.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxMD5.h"
#include "cxString.h"
#include "cxTypes.h"
#include "cxUtil.h"

static cxInt cxStringLuaPrint(lua_State *L)
{
    CX_LUA_DEF_THIS(cxString);
    if(this != NULL){
        CX_LOGGER("%s",cxStringBody(this));
    }
    return 0;
}

const luaL_Reg cxStringInstanceMethods[] = {
    {"print",cxStringLuaPrint},
    CX_LUA_SUPER(cxObject)
};

const luaL_Reg cxStringTypeMethods[] = {
    CX_LUA_TYPE(cxString)
};

void cxStringTypeInit()
{
    CX_LUA_LOAD_TYPE(cxString);
}

CX_OBJECT_INIT(cxString, cxObject)
{
    
}
CX_OBJECT_FREE(cxString, cxObject)
{
    utstring_done(&this->strptr);
}
CX_OBJECT_TERM(cxString, cxObject)

cxString cxMD5(cxString v)
{
    static const char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    CX_ASSERT(v != NULL, "args error");
    mongo_md5_state_t state={0};
    mongo_md5_byte_t digest[MD5_DIGEST_LENGTH];
    mongo_md5_init(&state);
    mongo_md5_append(&state, (const mongo_md5_byte_t *)cxStringBody(v), cxStringLength(v));
    mongo_md5_finish(&state, digest);
    cxChar md5[MD5_DIGEST_LENGTH*2 + 1]={0};
    for(cxInt i = 0; i < MD5_DIGEST_LENGTH; i++){
        md5[2*i] = hex[(digest[i] & 0xf0)>> 4];
        md5[2*i + 1] = hex[digest[i] & 0x0f];
    }
    return cxStringConstChars(md5);
}

void cxStringClean(cxString string)
{
    if(cxStringLength(string) > 0){
        utstring_clear(&string->strptr);
    }
}

cxBool cxStringEqu(cxString s1,cxString s2)
{
    if(s1 == NULL || s2 == NULL){
        return false;
    }
    if(s1->strptr.i != s2->strptr.i){
        return false;
    }
    return memcmp(s1->strptr.d, s2->strptr.d,s1->strptr.i) == 0;
}

cxString cxStringAllocChars(cxConstChars str)
{
    CX_ASSERT(str != NULL, "str null");
    cxString rv = CX_ALLOC(cxString);
    cxStringAppend(rv, (void *)str, (cxInt)strlen(str));
    return rv;
}

cxString cxStringAttachChars(cxChar *str)
{
    CX_ASSERT(str != NULL, "str null");
    return cxStringAttach(str, strlen(str));
}

cxString cxStringConstChars(cxConstChars str)
{
    CX_ASSERT(str != NULL, "str null");
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, (void *)str, (cxInt)strlen(str));
    return rv;
}

cxArray cxStringSplit(cxString string,cxConstChars sp)
{
    CX_ASSERT(string != NULL, "args error");
    cxConstChars body = cxStringBody(string);
    cxInt length = cxStringLength(string) + 1;
    cxChar buffer[length];
    cxInt idx = 0;
    cxArray ret = CX_CREATE(cxArray);
    for(cxInt i=0; i < length; i++){
        if(!cxConstCharsHasChar(sp, body[i]) && body[i] != '\0'){
            buffer[idx++] = body[i];
            continue;
        }
        if(idx == 0){
            continue;
        }
        cxString item = CX_CREATE(cxString);
        cxStringAppend(item, buffer, idx);
        cxArrayAppend(ret, item);
        idx = 0;
    }
    return ret;
}

cxConstChars cxStringBody(cxString string)
{
    CX_RETURN(string == NULL,NULL);
    return utstring_body(&string->strptr);
}

void cxStringConcat(cxString string,cxString str)
{
    utstring_concat(&string->strptr, &str->strptr);
}

cxInt cxStringLength(cxString string)
{
    CX_RETURN(string == NULL, 0);
    return utstring_len(&string->strptr);
}

cxString cxStringBinary(cxPointer d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, d, l);
    return rv;
}

cxString cxStringAttach(cxChar *d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    return rv;
}

cxString cxStringCreate(cxConstChars format,...)
{
    cxString rv = CX_CREATE(cxString);
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&rv->strptr, format, ap);
    va_end(ap);
    return rv;
}

void cxStringFormat(cxString string,cxConstChars format,...)
{
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&string->strptr, format, ap);
    va_end(ap);
}

void cxStringAppend(cxString string,cxConstChars d,cxInt l)
{
    CX_ASSERT(l > 0 && d != NULL, "args error");
    utstring_bincpy(&string->strptr, d, l);
}





