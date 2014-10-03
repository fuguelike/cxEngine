//
//  cxHttp.c
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEventBase.h"
#include "cxHttpConn.h"
#include "cxHttp.h"

cxString cxHttpUriEncode(cxString uri)
{
    CX_RETURN(uri == NULL, NULL);
    cxChars data = evhttp_uriencode(cxStringBody(uri), cxStringLength(uri), 0);
    CX_RETURN(data == NULL, NULL);
    return cxStringAttachMem(data, strlen(data));
}

cxString cxHttpUriDecode(cxString uri)
{
    cxInt size = 0;
    cxChars data = evhttp_uridecode(cxStringBody(uri), 0, (size_t *)&size);
    CX_RETURN(data == NULL, NULL);
    return cxStringAttachMem(data, size);
}

static void cxHttpReadData(cxHttp this)
{
    this->readBytes = this->request->body_size;
    if(this->bodyBytes == 0){
        cxConstChars cl = evhttp_find_header(this->request->input_headers, "Content-Length");
        this->bodyBytes = cl != NULL ? atoll(cl) : 0;
    }
    this->isSuccess = this->request->response_code == HTTP_OK;
    if(!this->isSuccess){
        return;
    }
    size_t len = evbuffer_get_length(this->request->input_buffer);
    if(len <= 0){
        return;
    }
    char *pdata = allocator->malloc(len);
    evbuffer_copyout(this->request->input_buffer, pdata, len);
    cxStringAppend(this->data, pdata, len);
    allocator->free(pdata);
}

static void cxHttpRequestCompleted(struct evhttp_request *req,void *xhttp)
{
    cxHttp this = xhttp;
    cxHttpReadData(this);
    CX_EVENT_FIRE(this, onCompleted);
}

static void cxHttpRequestChunked(struct evhttp_request *req,void *xhttp)
{
    cxHttp this = xhttp;
    cxStringClear(this->data);
    cxHttpReadData(this);
    CX_EVENT_FIRE(this, onChunked);
}

CX_OBJECT_TYPE(cxHttp, cxObject)
{
    
}
CX_OBJECT_INIT(cxHttp, cxObject)
{
    this->data = CX_ALLOC(cxString);
}
CX_OBJECT_FREE(cxHttp, cxObject)
{
    if(this->request != NULL){
        evhttp_cancel_request(this->request);
    }
    CX_EVENT_RELEASE(this->onCompleted);
    CX_EVENT_RELEASE(this->onChunked);
    if(this->uri != NULL){
        evhttp_uri_free(this->uri);
    }
    CX_RELEASE(this->data);
    CX_RELEASE(this->suri);
}
CX_OBJECT_TERM(cxHttp, cxObject)

static cxHttpConn cxHttpGetConnect(cxAny http)
{
    CX_ASSERT_THIS(http, cxHttp);
    cxConstChars host = evhttp_uri_get_host(this->uri);
    cxInt port = evhttp_uri_get_port(this->uri);
    return cxEventBaseHttpConnect(host,port < 0 ? 80 : port);
}

cxString cxHttpBody(cxAny http)
{
    CX_ASSERT_THIS(http, cxHttp);
    return this->data;
}

void cxHttpCancel(cxAny http)
{
    CX_ASSERT_THIS(http, cxHttp);
    evhttp_cancel_request(this->request);
}

cxInt cxHttpLength(cxAny http)
{
    CX_ASSERT_THIS(http, cxHttp);
    return (cxInt)this->bodyBytes;
}

static cxString cxHttpGetUri(cxAny http)
{
    CX_ASSERT_THIS(http, cxHttp);
    cxConstChars path = evhttp_uri_get_path(this->uri);
    cxConstChars query = evhttp_uri_get_query(this->uri);
    if(query != NULL){
        return cxStringCreate("%s?%s",path,query);
    }else if(cxConstCharsOK(path)){
        return cxStringCreate("%s",path);
    }else{
        return cxStringCreate("/");
    }
}

static cxBool cxHttpInit(cxAny http,cxConstChars uri,cxBool chunked)
{
    CX_ASSERT_THIS(http, cxHttp);
    this->uri = evhttp_uri_parse(uri);
    this->request = evhttp_request_new(cxHttpRequestCompleted, this);
    if(chunked){
        evhttp_request_set_chunked_cb(this->request, cxHttpRequestChunked);
    }
    evhttp_add_header(this->request->output_headers, "Host", evhttp_uri_get_host(this->uri));
    return true;
}

void cxHttpAddHeader(cxAny http,cxString key,cxString value)
{
    CX_ASSERT_THIS(http, cxHttp);
    CX_ASSERT(cxStringOK(key) && cxStringOK(value), "error key or value");
    evhttp_add_header(this->request->output_headers, cxStringBody(key), cxStringBody(value));
}

cxHttp cxHttpPost(cxConstChars url,cxString data,cxBool chunked)
{
    cxHttp this = CX_CREATE(cxHttp);
    if(!cxHttpInit(this,url,chunked)){
        CX_ERROR("cx http init error");
        return NULL;
    }
    if(cxStringOK(data)){
        evbuffer_add(this->request->output_buffer, cxStringBody(data), cxStringLength(data));
    }
    cxString suri = cxHttpGetUri(this);
    if(suri == NULL){
        CX_ERROR("get uri error");
        return NULL;
    }
    CX_RETAIN_SWAP(this->suri, suri);
    //run request
    cxHttpConn conn = cxHttpGetConnect(this);
    if(conn == NULL){
        CX_ERROR("get http connection error");
        return NULL;
    }
    if(evhttp_make_request(conn->conn, this->request, EVHTTP_REQ_POST, cxStringBody(this->suri)) != 0){
        CX_ERROR("make request error");
        return NULL;
    }
    return this;
}

cxHttp cxHttpGet(cxConstChars url,cxBool chunked)
{
    cxHttp this = CX_CREATE(cxHttp);
    if(!cxHttpInit(this,url,chunked)){
        CX_ERROR("cx http init error");
        return NULL;
    }
    cxString suri = cxHttpGetUri(this);
    if(suri == NULL){
        CX_ERROR("get uri error");
        return NULL;
    }
    CX_RETAIN_SWAP(this->suri, suri);
    //run request
    cxHttpConn conn = cxHttpGetConnect(this);
    if(conn == NULL){
        CX_ERROR("get http connection error");
        return NULL;
    }
    if(evhttp_make_request(conn->conn, this->request, EVHTTP_REQ_GET, cxStringBody(this->suri)) != 0){
        CX_ERROR("make request error");
        return NULL;
    }
    return this;
}

