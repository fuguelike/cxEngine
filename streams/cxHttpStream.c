//
//  cxHttpStream.c
//  cxEngine
//
//  Created by xuhua on 10/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxHttpStream.h"

static cxBool cxHttpStreamOpen(cxAny this)
{
    return false;
}

static cxInt cxHttpStreamRead(cxAny this,cxPointer buffer,cxInt size)
{
    return 0;
}

static cxInt cxHttpStreamWrite(cxAny this,cxPointer buffer,cxInt size)
{
    return 0;
}

static cxOff cxHttpStreamPosition(cxAny this)
{
    return 0;
}

static cxBool cxHttpStreamSeek(cxAny this,cxOff off,cxInt flags)
{

    return false;
}

static cxString cxHttpStreamAllBytes(cxAny this)
{
    return NULL;
}

static void cxHttpStreamClose(cxAny this)
{
    
}

static const cxStreamInterface httpInterface = {
    .Open       = cxHttpStreamOpen,
    .Read       = cxHttpStreamRead,
    .Write      = cxHttpStreamWrite,
    .Seek       = cxHttpStreamSeek,
    .Close      = cxHttpStreamClose,
    .Position   = cxHttpStreamPosition,
    .AllBytes   = cxHttpStreamAllBytes,
};

CX_OBJECT_INIT(cxHttpStream, cxStream)
{
    this->super.interface = &httpInterface;

}
CX_OBJECT_FREE(cxHttpStream, cxStream)
{

}
CX_OBJECT_TERM(cxHttpStream, cxStream)












