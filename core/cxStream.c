//
//  cxStream.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <libgen.h>
#include "cxStream.h"

CX_OBJECT_INIT(cxStream, cxObject)
{
}
CX_OBJECT_FREE(cxStream, cxObject)
{
    if(this->isOpen){
        this->interface->Close(this);
    }
    CX_RELEASE(this->path);
    CX_RELEASE(this->file);
}
CX_OBJECT_TERM(cxStream, cxObject)

void cxStreamClose(cxAny this)
{
    cxStream stream = this;
    stream->length = 0;
    stream->canRead = false;
    stream->canWrite = false;
    stream->canSeek = false;
    stream->isOpen = false;
}

cxString cxStreamFileDir(cxAny this)
{
    cxStream stream = this;
    if(stream->path == NULL){
        return cxStringCreate(".");
    }
    char *dir = dirname((char *)cxStringBody(stream->file));
    return cxStringCreate("%s",dir);
}












