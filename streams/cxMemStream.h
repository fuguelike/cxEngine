//
//  cxMemStream.h
//  cxEngine
//  内存流实现
//  Created by xuhua on 9/30/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxMemStream_h
#define cxEngine_cxMemStream_h

#include <cxcore/cxStream.h>

CX_C_BEGIN

CX_DEF(cxMemStream, cxStream)
    cxInt position;
    char *data;
    cxUInt allocSize;
    cxStr datas;
CX_END(cxMemStream, cxStream)

//not copy data
cxStream cxMemStreamCreateRefStr(cxStr data);

cxStream cxMemStreamCreateWithStr(cxStr txt);

CX_C_END

#endif
