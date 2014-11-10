//
//  cxTCP.c
//  cxEngine
//
//  Created by xuhua on 11/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxTCP.h"

//
static void release_close_cb(uv_handle_t* handle)
{
    CX_ASSERT_THIS(handle->data, cxTCP);
    this->IsConnected = false;
    CX_CALL(this, OnClose, CX_M(void));
    CX_SUPER(cxObject, this, cxRelease, CX_M(void));
}
//when get up from hostname
CX_METHOD_DEF(cxTCP, OnAddr, void,cxInt status,struct sockaddr *addr)
{
    char addrs[17] = {'\0'};
    if(uv_ip4_name((struct sockaddr_in*)addr, addrs, 16)== 0){
        CX_LOGGER("OnAddr %s", addrs);
    }
}
//when onconnected
CX_METHOD_DEF(cxTCP, OnConnected, void)
{
    CX_LOGGER("OnConnected");
}
//when closed
CX_METHOD_DEF(cxTCP, OnClose, void)
{
    CX_LOGGER("OnClose");
}
//when error
CX_METHOD_DEF(cxTCP, OnError, void, cxInt error)
{
    CX_LOGGER("OnError %d",error);
}
//when read data
CX_METHOD_DEF(cxTCP, OnRead, void, cxAny buffer,cxInt size)
{
    CX_LOGGER("OnRead %d",size);
}
CX_METHOD_DEF(cxTCP, cxRelease, void)
{
    uv_close((uv_handle_t *)&this->handle, release_close_cb);
}
CX_TYPE(cxTCP, cxObject)
{
    //free this, when tcp closed
    CX_METHOD(cxTCP, cxRelease);
    CX_METHOD(cxTCP, OnAddr);
    CX_METHOD(cxTCP, OnClose);
    CX_METHOD(cxTCP, OnError);
    CX_METHOD(cxTCP, OnRead);
    CX_METHOD(cxTCP, OnConnected);
}
CX_INIT(cxTCP, cxObject)
{
    cxLooper looper = cxEngineLooper();
    uv_tcp_init(&looper->looper, &this->handle);
    this->handle.data = this;
    this->hints.ai_family = AF_INET;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_protocol = IPPROTO_TCP;
    this->hints.ai_flags = 0;
    this->buffer = allocator->malloc(1024);
    this->bufsiz = 1024;
}
CX_FREE(cxTCP, cxObject)
{
    CX_RELEASE(this->Host);
    allocator->free(this->buffer);
}
CX_TERM(cxTCP, cxObject)

static void cxTCPError(cxAny ptcp,cxInt error)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    cxLooper looper = cxEngineLooper();
    CX_CALL(this, OnError, CX_M(void,cxInt),error);
    uv_tcp_init(&looper->looper, &this->handle);
}

static void alloc_cb(uv_handle_t* handle,size_t suggested,uv_buf_t* buf)
{
    CX_ASSERT_THIS(handle->data, cxTCP);
    if(suggested > this->bufsiz){
        this->buffer = allocator->realloc(this->buffer,suggested);
        this->bufsiz = (cxInt)suggested;
    }
    buf->base = this->buffer;
    buf->len = this->bufsiz;
}

static void read_cb(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf)
{
    CX_ASSERT_THIS(tcp->data, cxTCP);
    if (nread >= 0) {
        CX_CALL(this, OnRead, CX_M(void,cxAny,cxInt),buf->base,(cxInt)nread);
        return;
    }
    if(nread == UV_EOF){
        CX_CALL(this, OnClose, CX_M(void));
        return;
    }
    cxTCPError(this, (cxInt)nread);
}

static void connect_cb(uv_connect_t* req, int status)
{
    CX_ASSERT_THIS(req->data, cxTCP);
    if(status != 0){
        cxTCPError(this, (cxInt)status);
        return;
    }
    this->IsConnected = true;
    CX_CALL(this, OnConnected, CX_M(void));
    cxInt ret = uv_read_start(req->handle, alloc_cb, read_cb);
    if(ret != 0){
        cxTCPError(this, (cxInt)ret);
    }
}

static void resolved_cb(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res) {

    CX_ASSERT_THIS(resolver->data, cxTCP);
    CX_CALL(this, OnAddr, CX_M(void,cxInt,struct sockaddr *),status,res->ai_addr);
    //auto connect
    this->connreq.data = this;
    cxInt ret = uv_tcp_connect(&this->connreq, &this->handle, (struct sockaddr *)res->ai_addr, connect_cb);
    if(ret != 0){
        cxTCPError(this, (cxInt)ret);
    }
    uv_freeaddrinfo(res);
}

static cxBool cxTCPGetIpAddr(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    cxLooper looper = cxEngineLooper();
    this->resolver.data = this;
    cxConstChars port = cxConstString("%d",this->Port);
    cxConstChars host = cxStringBody(this->Host);
    return uv_getaddrinfo(&looper->looper,&this->resolver,resolved_cb,host,port,&this->hints) == 0;
}

static void write_cb(uv_write_t* req, int status)
{
    CX_ASSERT_TYPE(req->data, cxString);
    CX_RELEASE(req->data);
    allocator->free(req);
}

cxBool cxTCPWrite(cxAny ptcp,const cxString data)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    CX_ASSERT(cxStringOK(data), "args error");
    CX_RETURN(!this->IsConnected,false);
    uv_write_t *wreq = allocator->malloc(sizeof(uv_write_t));
    CX_RETAIN(data);
    wreq->data = data;
    uv_buf_t buf = uv_buf_init(cxStringBody(data),cxStringLength(data));
    if(uv_write(wreq, (uv_stream_t *)&this->handle, &buf, 1, write_cb) == 0){
        return true;
    }
    allocator->free(wreq);
    CX_RELEASE(data);
    return false;
}

cxBool cxTCPConnect(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    CX_ASSERT(cxStringOK(this->Host) && this->Port > 0, "args error");
    return cxTCPGetIpAddr(this);
}

