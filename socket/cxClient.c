//
//  cxClient.c
//  cxEngine
//
//  Created by xuhua on 11/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <evutil.h>
#include "cxClient.h"
#include "cxEventBase.h"

CX_OBJECT_TYPE(cxClient, cxObject)
{
    
}
CX_OBJECT_INIT(cxClient, cxObject)
{
    
}
CX_OBJECT_FREE(cxClient, cxObject)
{
    evutil_closesocket(this->socket);
    if(this->bufevent != NULL){
        bufferevent_free(this->bufevent);
        this->bufevent = NULL;
    }
    CX_RELEASE(this->host);
}
CX_OBJECT_TERM(cxClient, cxObject)

static evutil_socket_t cxCreateSocket(cxBool reuse)
{
	evutil_socket_t fd = -1;
	int on = 1;
	int serrno;
	/* Create listen socket */
	fd = socket(AF_INET, SOCK_STREAM, 0);
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

cxClient cxClientCreate(cxConstChars host,cxInt port)
{
    cxClient this = CX_CREATE(cxClient);
    this->port = port;
    this->host = cxStringAllocChars(host);
    return this;
}

static void cxClientReadCB(struct bufferevent *bev, void *ctx)
{
    CX_ASSERT_THIS(ctx, cxClient);
    struct evbuffer *input = bufferevent_get_input(bev);
    CX_METHOD_RUN(this->Read,this,input);
}

static void cxClientWriteCB(struct bufferevent *bev, void *ctx)
{
    CX_ASSERT_THIS(ctx, cxClient);
    struct evbuffer *output = bufferevent_get_output(bev);
    CX_METHOD_RUN(this->Write,this,output);
}

static void cxClientEventCB(struct bufferevent *bev, short what, void *ctx)
{
    CX_ASSERT_THIS(ctx, cxClient);
    CX_METHOD_RUN(this->Event,this,what);
}

cxInt cxClientWriteString(cxClient this,cxString data)
{
    if(!cxStringOK(data)){
        return 0;
    }
    return cxClientWriteBytes(this, (cxAny)cxStringBody(data), cxStringLength(data));
}

cxInt cxClientWriteBytes(cxClient this,cxAny buffer,cxInt size)
{
    struct evbuffer *output = bufferevent_get_output(this->bufevent);
    return evbuffer_add(output, buffer, size);
}

cxBool cxClientConnect(cxClient this)
{
    cxEventBase base = cxEventBaseInstance();
    this->socket = cxCreateSocket(true);
    if(this->socket < 0){
        CX_ERROR("create socket error");
        return false;
    }
    this->bufevent = bufferevent_socket_new(base->base, this->socket, 0);
    if(this->bufevent == NULL){
        CX_ERROR("make buffer event failed");
        return false;
    }
    bufferevent_setcb(this->bufevent, cxClientReadCB, cxClientWriteCB, cxClientEventCB, this);
    bufferevent_enable(this->bufevent, EV_READ|EV_WRITE);
    if(bufferevent_socket_connect_hostname(this->bufevent, NULL, AF_UNSPEC, cxStringBody(this->host), this->port) < 0){
        CX_ERROR("connect to %s:%d failed",cxStringBody(this->host),this->port);
        return false;
    }
    return true;
}


















