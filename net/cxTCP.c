//
//  cxTCP.c
//  cxEngine
//
//  Created by xuhua on 11/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxTCP.h"

//when get up from hostname
CX_METHOD_DEF(cxTCP, OnIpAddr, void,cxInt status,struct sockaddr *addr)
{
    char addrs[17] = {'\0'};
    uv_ip4_name((struct sockaddr_in*)addr, addrs, 16);
    CX_DELEGATE_RUN(this->Delegate, cxTCP, OnIpAddr, CX_M(void,cxAny,cxConstChars),this,addrs);
}
//when onconnected
CX_METHOD_DEF(cxTCP, OnConnected, void)
{
    CX_DELEGATE_RUN(this->Delegate, cxTCP, OnConnected, CX_M(void,cxAny),this);
}
//when closed
CX_METHOD_DEF(cxTCP, OnClose, void)
{
    CX_DELEGATE_RUN(this->Delegate, cxTCP, OnClose, CX_M(void,cxAny),this);
}
//when read data
CX_METHOD_DEF(cxTCP, OnRead, void, cxAny buffer,cxInt size)
{
    CX_DELEGATE_RUN(this->Delegate, cxTCP, OnRead, CX_M(void,cxAny,cxAny,cxInt),this,buffer,size);
}
CX_TYPE(cxTCP, cxObject)
{
    CX_METHOD(cxTCP, OnIpAddr);
    CX_METHOD(cxTCP, OnClose);
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
    this->buffer = allocator->malloc(4096);
    this->bufsiz = 4096;
}
CX_FREE(cxTCP, cxObject)
{
    CX_RELEASE(this->Delegate);
    CX_RELEASE(this->Data);
    CX_RELEASE(this->Host);
    allocator->free(this->buffer);
}
CX_TERM(cxTCP, cxObject)

static void shutdown_cb(uv_shutdown_t* req, int status)
{
    CX_ASSERT_THIS(req->data, cxTCP);
    this->IsShutdown = true;
}

//
static void close_cb(uv_handle_t* handle)
{
    CX_ASSERT_THIS(handle->data, cxTCP);
    this->IsConnected = false;
    CX_CALL(this, OnClose, CX_M(void));
    //release this at close
    CX_RELEASE(this);
}

void cxTCPShutdown(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    if(!uv_is_closing((uv_handle_t *)&this->handle)){
        this->shutreq.data = this;
        uv_shutdown(&this->shutreq, (uv_stream_t *)&this->handle, shutdown_cb);
    }
}

void cxTCPClose(cxAny ptcp,cxInt error)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    this->Error = error;
    if(!uv_is_closing((uv_handle_t *)&this->handle)){
        uv_close((uv_handle_t *)&this->handle, close_cb);
    }
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
        if(uv_is_readable(tcp)){
            CX_CALL(this, OnRead, CX_M(void,cxAny,cxInt),buf->base,(cxInt)nread);
        }
    }else{
        cxTCPClose(this, (cxInt)nread);
    }
}

static void connect_cb(uv_connect_t* req, int status)
{
    CX_ASSERT_THIS(req->data, cxTCP);
    if(status != 0){
        cxTCPClose(this, (cxInt)status);
        return;
    }
    this->IsConnected = true;
    CX_CALL(this, OnConnected, CX_M(void));
    cxInt ret = uv_read_start(req->handle, alloc_cb, read_cb);
    if(ret != 0){
        cxTCPClose(this, (cxInt)ret);
    }
}

static void resolved_cb(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res) {

    CX_ASSERT_THIS(resolver->data, cxTCP);
    if(status == 0){
        CX_CALL(this, OnIpAddr, CX_M(void,cxInt,struct sockaddr *),status,res->ai_addr);
        //auto connect
        this->connreq.data = this;
        cxInt ret = uv_tcp_connect(&this->connreq, &this->handle, (struct sockaddr *)res->ai_addr, connect_cb);
        if(ret != 0){
            cxTCPClose(this, (cxInt)ret);
        }
    }else{
        cxTCPClose(this, (cxInt)status);
    }
    uv_freeaddrinfo(res);
}

static cxBool cxTCPGetIpAddr(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    cxLooper looper = cxEngineLooper();
    this->resolver.data = this;
    cxConstChars port = cxConstStr("%d",this->Port);
    cxConstChars host = cxStrBody(this->Host);
    cxInt ret = uv_getaddrinfo(&looper->looper,&this->resolver,resolved_cb,host,port,&this->hints);
    if(ret != 0){
        cxTCPClose(this, (cxInt)ret);
    }
    return ret == 0;
}

static void uv_connection(uv_stream_t* server, int status)
{
    if(status != 0){
        CX_ERROR("uv connection status %d",status);
        return;
    }
}

cxBool cxTCPListen(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    struct sockaddr_in addr = {0};
    uv_ip4_addr(cxStrBody(this->Host), this->Port, &addr);
    if(uv_tcp_bind(&this->handle, (struct sockaddr*)&addr, 0) != 0){
        return false;
    }
    if(uv_listen((uv_stream_t *)&this->handle, 16, uv_connection) != 0){
        return false;
    }
    return true;
}

static void write_cb(uv_write_t* req, int status)
{
    CX_ASSERT_TYPE(req->data, cxStr);
    CX_RELEASE(req->data);
    allocator->free(req);
}

cxBool cxTCPWrite(cxAny ptcp,const cxStr data)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    CX_ASSERT(cxStrOK(data), "args error");
    CX_RETURN(!this->IsConnected,false);
    if(!uv_is_writable((uv_stream_t *)&this->handle)){
        return false;
    }
    uv_write_t *wreq = allocator->malloc(sizeof(uv_write_t));
    CX_RETAIN_SET(wreq->data, data);
    uv_buf_t buf = uv_buf_init(cxStrBody(data),cxStrLength(data));
    cxInt ret = uv_write(wreq, (uv_stream_t *)&this->handle, &buf, 1, write_cb);
    if(ret == 0){
        return true;
    }
    cxTCPClose(this, (cxInt)ret);
    allocator->free(wreq);
    CX_RELEASE(data);
    return false;
}

cxBool cxTCPConnect(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    CX_ASSERT(cxStrOK(this->Host) && this->Port > 0, "args error");
    return cxTCPGetIpAddr(this);
}



