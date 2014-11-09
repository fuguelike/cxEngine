//
//  cxStream.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxStream.h"

CX_METHOD_DEF(cxStream,Open,cxBool)
{
    return false;
}
CX_METHOD_DEF(cxStream, Read, cxInt, cxAny buff, cxInt bytes)
{
    return 0;
}
CX_METHOD_DEF(cxStream, Write, cxInt, cxAny buff, cxInt bytes)
{
    return 0;
}
CX_METHOD_DEF(cxStream, Seek, cxInt, cxInt seek, cxInt where)
{
    return 0;
}
CX_METHOD_DEF(cxStream, Position, cxInt)
{
    return 0;
}
CX_METHOD_DEF(cxStream, Close,void)
{
    this->Length = 0;
    this->canRead = false;
    this->canWrite = false;
    this->canSeek = false;
    this->isOpen = false;
}
CX_METHOD_DEF(cxStream, AllBytes,cxString)
{
    return NULL;
}
CX_TYPE(cxStream, cxObject)
{
    CX_METHOD(cxStream, Open);
    CX_METHOD(cxStream, Read);
    CX_METHOD(cxStream, Write);
    CX_METHOD(cxStream, Seek);
    CX_METHOD(cxStream, Position);
    CX_METHOD(cxStream, Close);
    CX_METHOD(cxStream, AllBytes);
}
CX_INIT(cxStream, cxObject)
{
    //
}
CX_FREE(cxStream, cxObject)
{
    CX_CALL(this, Close, CX_M(void));
    CX_RELEASE(this->path);
}
CX_TERM(cxStream, cxObject)













