//
//  cxMp3Stream.h
//  cxEngine
//
//  Created by xuhua on 11/1/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngineIOS_cxMp3Stream_h
#define cxEngineIOS_cxMp3Stream_h

#include <mpg123.h>
#include <core/cxBase.h>
#include <core/cxStream.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxMp3Stream, cxStream)
    cxString path;
    mpg123_handle *mh;
    cxInt fd;
    cxPointer buffer;
    cxInt bufsiz;
    cxInt encoding;
    cxInt channels;
    cxLong freq;
CX_OBJECT_END(cxMp3Stream)

cxStream cxMp3StreamCreate(cxConstChars file);

cxPointer cxMp3StreamBuffer(cxAny mp3,cxInt *bytes);

CX_C_END

#endif
