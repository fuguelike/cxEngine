//
//  cxStream.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxStream.h"

static cxBool CX_METHOD(cxStream,Open)
{
    return false;
}
static cxInt CX_METHOD(cxStream, Read, cxAny buff, cxInt bytes)
{
    return 0;
}
static cxInt CX_METHOD(cxStream, Write, cxAny buff, cxInt bytes)
{
    return 0;
}
static cxInt CX_METHOD(cxStream, Seek, cxOff seek, cxInt where)
{
    return 0;
}
static cxOff CX_METHOD(cxStream, Position)
{
    return 0;
}
static void CX_METHOD(cxStream, Close)
{
    this->Length = 0;
    this->canRead = false;
    this->canWrite = false;
    this->canSeek = false;
    this->isOpen = false;
}
static cxString CX_METHOD(cxStream, AllBytes)
{
    return NULL;
}

CX_TYPE(cxStream, cxObject)
{
    CX_MSET(cxStream, Open);
    CX_MSET(cxStream, Read);
    CX_MSET(cxStream, Write);
    CX_MSET(cxStream, Seek);
    CX_MSET(cxStream, Position);
    CX_MSET(cxStream, Close);
    CX_MSET(cxStream, AllBytes);
}
CX_INIT(cxStream, cxObject)
{
    //
}
CX_FREE(cxStream, cxObject)
{
    CX_CALL(this, Close, CX_MT(void));
    CX_RELEASE(this->path);
}
CX_TERM(cxStream, cxObject)













