//
//  cxLooper.h
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLooper_h
#define cxEngine_cxLooper_h

#include <event.h>
#include <cxCore/cxBase.h>

CX_C_BEGIN

#ifndef CX_EVLOOP_FREQ
#define CX_EVLOOP_FREQ    0.01f
#endif

CX_DEF(cxLooper, cxObject)
    cxHash conns;   //host:port key
    struct event_base *looper;
    CX_SLOT_ALLOC(onUpdate);
CX_END(cxLooper, cxObject)

void cxLooperUpdate(cxAny looper);

//type=SOCK_DGRAM udp,or SOCK_STREAM tcp
evutil_socket_t cxCreateSocket(cxBool reuse,int type);

cxAny cxLooperHttpConnect(cxConstChars host,cxInt port);

CX_C_END

#endif
