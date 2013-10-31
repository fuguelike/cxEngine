//
//  cxEventBase.h
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEventBase_h
#define cxEngine_cxEventBase_h

#include <event.h>
#include <core/cxBase.h>
#include <core/cxHash.h>
#include "cxHttpConn.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxEventBase, cxObject)
    cxHash httpconns;   //host:port key
    struct event_base *base;
    CX_SLOT_ALLOC(onUpdate);
CX_OBJECT_END(cxEventBase)

cxEventBase cxEventBaseInstance();

void cxEventBaseDestroy();

cxHttpConn cxEventBaseHttpConnect(cxConstChars host,cxInt port);

CX_C_END

#endif
