//
//  cxUDP.c
//  cxCore
//
//  Created by xuhua on 10/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <evutil.h>
#include "cxLooper.h"
#include "cxUDP.h"

CX_TYPE(cxUDP, cxObject)
{
    
}
CX_INIT(cxUDP, cxObject)
{
    this->toAddrLen = sizeof(this->udpTo);
    memset(&this->udpTo, 0, this->toAddrLen);
    this->udpTo.sin_family = AF_INET;
}
CX_FREE(cxUDP, cxObject)
{
    if(this->socket > 0){
        evutil_closesocket(this->socket);
    }
    if(this->udpEvent != NULL){
        event_del(this->udpEvent);
        event_free(this->udpEvent);
    }
    CX_EVENT_RELEASE(this->onData);
    CX_RELEASE(this->host);
}
CX_TERM(cxUDP, cxObject)

cxInt cxUDPWriteString(cxAny pudp,const cxString data)
{
    CX_ASSERT_THIS(pudp, cxUDP);
    CX_ASSERT(cxStringOK(data), "data args error");
    cxInt l = cxStringLength(data);
    cxConstChars d = cxStringBody(data);
    return cxUDPWrite(this, d, l);
}

cxInt cxUDPWrite(cxAny pudp,cxConstChars data,cxInt len)
{
    CX_ASSERT_THIS(pudp, cxUDP);
    return (cxInt)sendto(this->socket, data, len, 0, (struct sockaddr *)&this->udpTo, this->toAddrLen);
}

static void UDPRead(evutil_socket_t sockt, short which, void *arg)
{
    CX_ASSERT_THIS(arg, cxUDP);
    if(!(which & EV_READ)){
        return;
    }
    this->bytes = (cxInt)recvfrom(this->socket, this->buffer, CX_UDP_BUFFER_SIZE, 0, (struct sockaddr *)&this->udpFrom, &this->fromAddrLen);
    if(this->bytes <= 0) {
        return;
    }
    this->buffer[this->bytes] = '\0';
    CX_EVENT_FIRE(this, onData);
}

cxUDP cxUDPCreate(cxConstChars host,cxInt port)
{
    cxLooper looper = cxLooperInstance();
    cxUDP this = CX_CREATE(cxUDP);
    this->host = cxStringAllocChars(host);
    this->port = port;
    this->socket = cxCreateSocket(true,SOCK_DGRAM);
    if(this->socket == -1){
        CX_ERROR("create udp socket error %d:%s",port,host);
        return NULL;
    }
    this->udpTo.sin_port = htons(port);
    this->udpTo.sin_addr.s_addr = inet_addr(host);
    this->udpEvent = event_new(looper->looper, this->socket, EV_READ | EV_PERSIST, UDPRead, this);
    if(event_add(this->udpEvent, NULL) != 0){
        CX_ERROR("add event error");
        return NULL;
    }
    return this;
}








