//
//  cxEventBase.c
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxEventBase.h"
#include "cxHttpConn.h"

evutil_socket_t cxCreateSocket(cxBool reuse,int type)
{
    int on = 1;
    int serrno;
    evutil_socket_t fd = socket(AF_INET, type, 0);
    if (fd == -1) {
        CX_ERROR("socket create error");
        return (-1);
    }
    if (evutil_make_socket_nonblocking(fd) < 0){
        CX_ERROR("socket set nonblock error");
        goto out;
    }
    if (evutil_make_socket_closeonexec(fd) < 0){
        CX_ERROR("socket set exec close error");
        goto out;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&on, sizeof(on))<0){
        CX_ERROR("socket set keepalive error");
        goto out;
    }
    if (reuse && evutil_make_listen_socket_reuseable(fd) < 0) {
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

static cxEventBase instance = NULL;

cxEventBase cxEventBaseInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxEventBase);
    }
    return instance;
}

void cxEventBaseDestroy()
{
    CX_RELEASE(instance);
    instance = NULL;
}

void cxEventBaseSetFreq(cxInt freq)
{
    CX_ASSERT(instance != NULL, "event base not init");
    instance->freq = freq;
}

static void cxEventUpdate(cxAny base)
{
    cxEventBase this = base;
    this->counter ++;
    CX_RETURN((this->counter % this->freq) != 0);
    event_base_loop(this->base, EVLOOP_NONBLOCK);
}

CX_OBJECT_TYPE(cxEventBase, cxObject)
{

}
CX_OBJECT_INIT(cxEventBase, cxObject)
{
    cxEngine engine = cxEngineInstance();
    CX_LIN(cxEngine, engine, onUpdate, this, cxEventUpdate);
    this->base = event_base_new();
    this->conns = CX_ALLOC(cxHash);
    this->counter = 0;
    this->freq = 1;
}
CX_OBJECT_FREE(cxEventBase, cxObject)
{
    CX_RELEASE(this->conns);
    event_base_loopbreak(this->base);
    event_base_free(this->base);
    CX_SLOT_RELEASE(this->onUpdate);
}
CX_OBJECT_TERM(cxEventBase, cxObject)

cxAny cxEventBaseHttpConnect(cxConstChars host,cxInt port)
{
    cxEventBase this = cxEventBaseInstance();
    cxConstChars key = CX_CONST_STRING("%s:%d",host,port);
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


