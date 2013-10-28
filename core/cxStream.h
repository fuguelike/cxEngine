//
//  cxStream.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxStream_h
#define cxEngine_cxStream_h

#include "cxBase.h"
#include "cxString.h"

CX_C_BEGIN

typedef struct {
    cxBool      (*Open)(cxAny this);
    cxInt       (*Read)(cxAny this,cxPointer buffer,cxInt size);
    cxInt       (*Write)(cxAny this,cxPointer buffer,cxInt size);
    cxBool      (*Seek)(cxAny this,cxOff off,cxInt flags);
    cxOff       (*Position)(cxAny this);
    void        (*Close)(cxAny this);
    cxString    (*AllBytes)(cxAny this);
}cxStreamInterface;

CX_OBJECT_DEF(cxStream, cxObject)
    cxString path;
    cxString file;
    cxSize length;
    cxBool canRead;
    cxBool canWrite;
    cxBool canSeek;
    cxBool isOpen;
    const cxStreamInterface *interface;
CX_OBJECT_END(cxStream)

void cxStreamClose(cxAny this);

cxString cxStreamFileDir(cxAny this);

cxString cxSreamBytes(cxAny this);

CX_C_END

#endif










