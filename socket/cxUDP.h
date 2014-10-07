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

#define CX_UDP_BUFFER_SIZE 1024

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
    //on data readed
    cxChar buffer[CX_UDP_BUFFER_SIZE];
    cxInt bytes;
    CX_EVENT_ALLOC(onData);
CX_OBJECT_END(cxUDP, cxObject)

CX_INLINE cxAny cxUDPGetBuffer(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxUDP);
    return this->buffer;
}

CX_INLINE cxInt cxUDPGetBytes(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxUDP);
    return this->bytes;
}

cxUDP cxUDPCreate(cxConstChars host,cxInt port);

cxInt cxUDPWrite(cxAny pudp,cxConstChars data,cxInt len);

cxInt cxUDPWriteString(cxAny pudp,const cxString data);

CX_C_END

#endif
