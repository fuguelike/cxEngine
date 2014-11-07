//
//  cxTCP.c
//  cxEngineStatic
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <evutil.h>
#include "cxLooper.h"
#include "cxTCP.h"

CX_TYPE(cxTCP, cxObject)
{
    
}
CX_INIT(cxTCP, cxObject)
{

}
CX_FREE(cxTCP, cxObject)
{
    CX_EVENT_RELEASE(this->onRead);
    CX_EVENT_RELEASE(this->onError);
    CX_EVENT_RELEASE(this->onClose);
    CX_EVENT_RELEASE(this->onConnected);
    cxTcpBufferClose(this);
    CX_RELEASE(this->host);
}
CX_TERM(cxTCP, cxObject)

cxBool cxTcpWrite(cxAny ptcp,cxString data)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    if(!cxStringOK(data)){
        return false;
    }
    cxConstChars buf = cxStringBody(data);
    cxInt32 len = cxStringLength(data);
    //write package length
    if(bufferevent_write(this->bufferEvent, &len, sizeof(cxInt32)) != 0){
        return false;
    }
    //write package body data
    if(bufferevent_write(this->bufferEvent, buf, len) != 0){
        return false;
    }
    return true;
}

void cxTcpClose(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    this->IsConnected = false;
    CX_EVENT_FIRE(this, onClose);
    cxTcpBufferClose(this);
}

static void cxTcpBufferRead(struct bufferevent *bev, void *ctx)
{
    CX_ASSERT_THIS(ctx, cxTCP);
    this->bytes = (cxInt)bufferevent_read(this->bufferEvent, this->buf, CX_TCP_BUFFER_SIZE);
    if(this->bytes > 0){
        CX_EVENT_FIRE(this, onRead);
    }
}

//close need reconnect
void cxTcpBufferClose(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    CX_RETURN(this->bufferEvent == NULL);
    bufferevent_free(this->bufferEvent);
    this->bufferEvent = NULL;
}

static void cxTcpBufferEvent(struct bufferevent *bev, short what, void *ctx)
{
    //BEV_EVENT_TIMEOUT
    CX_ASSERT_THIS(ctx, cxTCP);
    if(what & BEV_EVENT_CONNECTED){
        this->IsConnected = true;
        CX_EVENT_FIRE(this, onConnected);
    }
    if(what & BEV_EVENT_EOF){
        this->IsConnected = false;
        CX_EVENT_FIRE(this, onClose);
        cxTcpBufferClose(this);
    }
    if(what & BEV_EVENT_ERROR){
        this->IsConnected = false;
        CX_EVENT_FIRE(this, onError);
        cxTcpBufferClose(this);
    }
}

cxBool cxTcpConnect(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    CX_ASSERT(cxStringOK(this->host) || this->port == 0, "tcp host or port not set");
    cxLooper looper = cxLooperInstance();
    cxConstChars host = cxStringBody(this->host);
    this->socket = cxCreateSocket(true,SOCK_STREAM);
    if(this->socket == -1){
        CX_ERROR("create tcp socket error %s:%d",host,this->port);
        return false;
    }
    this->bufferEvent = bufferevent_socket_new(looper->looper, this->socket, BEV_OPT_CLOSE_ON_FREE);
    if(this->bufferEvent == NULL){
        CX_ERROR("create buffer event socket failed");
        return false;
    }
    bufferevent_setcb(this->bufferEvent, cxTcpBufferRead, NULL, cxTcpBufferEvent, this);
    bufferevent_enable(this->bufferEvent, EV_READ|EV_WRITE);
    return  bufferevent_socket_connect_hostname(this->bufferEvent, NULL, AF_INET, host , this->port) == 0;
}

cxTCP cxTCPCreate(cxConstChars host,cxInt port)
{
    cxTCP this = CX_CREATE(cxTCP);
    cxTcpSetHost(this, host);
    cxTcpSetPort(this, port);
    return this;
}





