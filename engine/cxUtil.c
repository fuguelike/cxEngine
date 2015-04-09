//
//  cxUtil.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <engine/cxEngine.h>
#include <sys/stat.h>
#include "cxUtil.h"

cxBool cxWriteFile(cxStr file,cxStr data)
{
    FILE *fd = fopen(cxStrBody(file), "wb");
    CX_RETURN(fd == NULL, false);
    cxInt bytes = (cxInt)fwrite(cxStrBody(data), 1, cxStrLength(data), fd);
    fclose(fd);
    return bytes == cxStrLength(data);
}

cxInt cxFileFD(cxConstChars file,cxInt *off,cxInt *length)
{
    CX_ASSERT(off != NULL && length != NULL && file != NULL, "args error");
    struct stat stat={0};
    cxConstChars body = file;
    if(lstat(body, &stat) != 0){
        return -1;
    }
    *length = (cxInt)stat.st_size;
    *off = 0;
    return open(file, O_RDONLY,00644);
}

cxInt cxDocumentFD(cxConstChars file,cxInt *off,cxInt *length)
{
    CX_ASSERT(off != NULL && length != NULL && file != NULL, "args error");
    cxStr path = cxDocumentPath(file);
    CX_ASSERT(path != NULL, "get file %s path failed",file);
    return cxFileFD(cxStrBody(path), off, length);
}

cxBool cxDocumentExists(cxConstChars file)
{
    cxStr path = cxDocumentPath(file);
    CX_ASSERT(path != NULL, "get file %s path failed",file);
    return cxFileExists(cxStrBody(path));
}

cxBool cxFileExists(cxConstChars file)
{
    struct stat stat={0};
    return lstat(file, &stat) == 0;
}

cxBool cxCopyFile(cxConstChars file,cxCopyFileFunc func,cxAny udata)
{
    cxStream src = cxAssetsStreamCreate(file);
    cxStream dst = cxFileStreamCreate(file, false);
    if(!CX_CALL(src, Open, CX_M(cxBool))){
        return false;
    }
    if(!CX_CALL(dst, Open, CX_M(cxBool))){
        return false;
    }
    cxBool ret = true;
    cxChar buffer[4096];
    cxProgressInfo p = {0};
    p.current = 0;
    p.total = src->Length;
    while (true) {
        cxInt rbytes = CX_CALL(src, Read, CX_M(cxInt,cxAny,cxInt),buffer,4096);
        if(rbytes <= 0){
            break;
        }
        p.current += rbytes;
        if(CX_CALL(dst, Write, CX_M(cxInt,cxAny,cxInt),buffer,rbytes) != rbytes){
            CX_ERROR("write file error");
            ret = false;
            break;
        }
        if(func != NULL){
            func(file,&p,udata);
        }
    }
    return ret;
}

cxUInt cxBinaryToUInt(cxConstChars bs)
{
    CX_ASSERT(bs != NULL, "args error");
    cxInt len = (cxInt)strlen(bs);
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

cxUInt cxHexToUInt(cxConstChars bs)
{
    CX_ASSERT(bs != NULL, "args error");
    cxInt len = (cxInt)strlen(bs);
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
    if(cxConstCharsEqu(ext, ".pcm")){
        return cxAudioFileTypePCM;
    }
    if(cxConstCharsEqu(ext, ".mp3")){
        return cxAudioFileTypeMP3;
    }
    if(cxConstCharsEqu(ext, ".wav")){
        return cxAudioFileTypeWAV;
    }
    return cxAudioFileTypeNone;
}

