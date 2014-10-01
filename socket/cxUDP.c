//
//  cxUDP.c
//  cxCore
//
//  Created by xuhua on 10/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <evutil.h>
#include "cxEventBase.h"
#include "cxUDP.h"

CX_OBJECT_TYPE(cxUDP, cxObject)
{
    
}
CX_OBJECT_INIT(cxUDP, cxObject)
{
    this->toAddrLen = sizeof(struct sockaddr_in);
    memset(&this->udpTo, 0, sizeof(struct sockaddr_in));
    this->udpTo.sin_family = AF_INET;
}
CX_OBJECT_FREE(cxUDP, cxObject)
{
    if(this->udpEvent != NULL){
        event_free(this->udpEvent);
    }
    if(this->socket > 0){
        evutil_closesocket(this->socket);
    }
    CX_RELEASE(this->host);
}
CX_OBJECT_TERM(cxUDP, cxObject)

evutil_socket_t cxUDPSocket(cxBool reuse)
{
    evutil_socket_t fd = -1;
    int on = 1;
    int serrno;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        return (-1);
    }
    if (evutil_make_socket_nonblocking(fd) < 0){
        goto out;
    }
    if (evutil_make_socket_closeonexec(fd) < 0){
        goto out;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&on, sizeof(on))<0){
        goto out;
    }
    if (reuse && evutil_make_listen_socket_reuseable(fd) < 0) {
        goto out;
    }
    return (fd);
out:
    serrno = EVUTIL_SOCKET_ERROR();
    evutil_closesocket(fd);
    EVUTIL_SET_SOCKET_ERROR(serrno);
    return (-1);
}

cxInt cxUDPWriteString(cxAny pudp,cxString data)
{
    CX_ASSERT_THIS(pudp, cxUDP);
    if(!cxStringOK(data)){
        return 0;
    }
    cxInt l = cxStringLength(data);
    cxConstChars d = cxStringBody(data);
    return cxUDPWrite(pudp, d, l);
    
}

cxInt cxUDPWrite(cxAny pudp,cxConstChars data,cxInt len)
{
    CX_ASSERT_THIS(pudp, cxUDP);
    return sendto(this->socket, data, len, 0, (struct sockaddr *)&this->udpTo, this->toAddrLen);
}

static void UDPRead(evutil_socket_t sockt, short which, void *arg)
{
    CX_ASSERT_THIS(arg, cxUDP);
    cxChar buf[1024];
    struct sockaddr_in udpFrom;
    memset(&udpFrom, 0, sizeof(udpFrom));
    socklen_t fromAddrLen = sizeof(struct sockaddr_in);
    cxInt bytes = recvfrom(this->socket, buf, 1024, 0, (struct sockaddr *)&udpFrom, &fromAddrLen);
    if(bytes <= 0) {
        return;
    }
    CX_METHOD_RUN(this->OnData,this,buf,bytes);
}

cxUDP cxUDPCreate(cxConstChars host,cxInt port)
{
    cxEventBase base = cxEventBaseInstance();
    cxUDP this = CX_CREATE(cxUDP);
    this->host = cxStringAllocChars(host);
    this->port = port;
    this->socket = cxUDPSocket(true);
    if(this->socket == -1){
        CX_ERROR("create udp error %d:%s",port,host);
        return NULL;
    }
    struct in_addr addr;
    inet_pton(AF_INET, host, (void *)&addr);
    this->udpTo.sin_addr.s_addr = addr.s_addr;
    this->udpTo.sin_port = htons(port);
    this->udpEvent = event_new(base->base, this->socket, EV_READ | EV_PERSIST, UDPRead, this);
    if(event_add(this->udpEvent, NULL) != 0){
        CX_ERROR("add event error");
        return NULL;
    }
    return this;
}








