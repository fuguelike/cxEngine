//
//  cxUDP.h
//  cxCore
//
//  Created by xuhua on 10/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxUDP_h
#define cxEngine_cxUDP_h

#include <arpa/inet.h>
#include <unistd.h>
#include <event.h>
#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxUDP, cxObject)
    evutil_socket_t socket;
    cxInt port;
    cxString host;
    struct event *udpEvent;
    //to addr
    struct sockaddr_in udpTo;
    socklen_t toAddrLen;
    //from addr
    struct sockaddr_in udpFrom;
    socklen_t fromAddrLen;
    CX_METHOD_DEF(void, OnData,cxAny pudp,cxChars buf,cxInt size);
CX_OBJECT_END(cxUDP, cxObject)

cxUDP cxUDPCreate(cxConstChars host,cxInt port);

cxInt cxUDPWrite(cxAny pudp,cxConstChars data,cxInt len);

cxInt cxUDPWriteString(cxAny pudp,const cxString data);

CX_C_END

#endif
