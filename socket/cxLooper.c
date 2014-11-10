//
//  cxLooper.c
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <actions/cxTimer.h>
#include "cxLooper.h"
#include "cxHttpConn.h"

evutil_socket_t cxCreateSocket(cxBool reuse,int type)
{
    int on = 1;
    int serrno;
    evutil_socket_t fd = socket(AF_INET, type, IPPROTO_IP);
    if (fd == -1) {
        CX_ERROR("socket create error");
        return (-1);
    }
    if (evutil_make_socket_nonblocking(fd) != 0){
        CX_ERROR("socket set nonblock error");
        goto out;
    }
    if (evutil_make_socket_closeonexec(fd) != 0){
        CX_ERROR("socket set exec close error");
        goto out;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&on, sizeof(on)) != 0){
        CX_ERROR("socket set keepalive error");
        goto out;
    }
    if (reuse && evutil_make_listen_socket_reuseable(fd) != 0) {
        CX_ERROR("socket set reuseable error");
        goto out;
    }
    return (fd);
out:
    serrno = EVUTIL_SOCKET_ERROR();
    evutil_closesocket(fd);
    EVUTIL_SET_SOCKET_ERROR(serrno);
    return (-1);
}

void cxLooperUpdate(cxAny looper)
{
    CX_ASSERT_THIS(looper, cxLooper);
    event_base_loop(this->looper, EVLOOP_NONBLOCK);
}

CX_TYPE(cxLooper, cxObject)
{
    
}
CX_INIT(cxLooper, cxObject)
{
    this->looper = event_base_new();
    this->conns = CX_ALLOC(cxHash);
}
CX_FREE(cxLooper, cxObject)
{
    CX_SLOT_RELEASE(this->onUpdate);
    CX_RELEASE(this->conns);
    event_base_loopbreak(this->looper);
    event_base_free(this->looper);
}
CX_TERM(cxLooper, cxObject)

//use one connection
cxAny cxLooperHttpConnect(cxConstChars host,cxInt port)
{
    cxLooper this = cxLooperInstance();
    cxConstChars key = cxConstString("%s:%d",host,port);
    cxHttpConn conn = cxHashGet(this->conns, cxHashStrKey(key));
    if(conn != NULL){
        return conn;
    }
    conn = cxHttpConnectOpen(host, port);
    if(conn == NULL){
        CX_ERROR("http open %s:%d failed",host,port);
        return NULL;
    }
    cxHashSet(this->conns, cxHashStrKey(key), conn);
    return conn;
}


