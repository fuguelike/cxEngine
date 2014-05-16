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
    cxChar *data = evhttp_uriencode(cxStringBody(uri), cxStringLength(uri), 0);
    CX_RETURN(data == NULL, NULL);
    return cxStringAttach(data, strlen(data));
}

cxString cxHttpUriDecode(cxString uri)
{
    cxInt size = 0;
    cxChar *data = evhttp_uridecode(cxStringBody(uri), 0, (size_t *)&size);
    CX_RETURN(data == NULL, NULL);
    return cxStringAttach(data, size);
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
    cxStringClean(this->data);
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
    CX_EVENT_RELEASE(this->onChunked);
    CX_RELEASE(this->data);
    if(this->uri != NULL){
        evhttp_uri_free(this->uri);
    }
    CX_EVENT_RELEASE(this->onCompleted);
}
CX_OBJECT_TERM(cxHttp, cxObject)

static cxHttpConn cxHttpGetConnect(cxAny http)
{
    cxHttp this = http;
    cxConstChars host = evhttp_uri_get_host(this->uri);
    cxInt port = evhttp_uri_get_port(this->uri);
    return cxEventBaseHttpConnect(host,port < 0 ? 80 : port);
}

cxString cxHttpBody(cxAny http)
{
    cxHttp this = http;
    return this->data;
}

void cxHttpCancel(cxAny http)
{
    cxHttp this = http;
    evhttp_cancel_request(this->request);
}

cxInt cxHttpLength(cxAny http)
{
    cxHttp this = http;
    return (cxInt)this->bodyBytes;
}

static cxString cxHttpGetUri(cxAny http)
{
    cxHttp this = http;
    cxConstChars path = evhttp_uri_get_path(this->uri);
    cxConstChars query = evhttp_uri_get_query(this->uri);
    if(query != NULL){
        return cxStringCreate("%s?%s",path,query);
    }else{
        return cxStringCreate("%s",path);
    }
}

static cxBool cxHttpInit(cxAny http,cxConstChars uri,cxBool chunked)
{
    cxHttp this = http;
    this->uri = evhttp_uri_parse(uri);
    this->request = evhttp_request_new(cxHttpRequestCompleted, this);
    if(chunked){
        evhttp_request_set_chunked_cb(this->request, cxHttpRequestChunked);
    }
    evhttp_add_header(this->request->output_headers, "Host", evhttp_uri_get_host(this->uri));
    evhttp_add_header(this->request->output_headers, "Referer", ".google.com");
    return true;
}

cxHttp cxHttpPost(cxConstChars url,cxString data,cxBool chunked)
{
    cxHttp this = CX_CREATE(cxHttp);
    if(!cxHttpInit(this,url,chunked)){
        CX_ERROR("cx http init error");
        return NULL;
    }
    cxHttpConn conn = cxHttpGetConnect(this);
    if(conn == NULL){
        CX_ERROR("get http connection error");
        return NULL;
    }
    cxString suri = cxHttpGetUri(this);
    if(suri == NULL){
        CX_ERROR("get uri error");
        return NULL;
    }
    if(cxStringOK(data)){
        evbuffer_add(this->request->output_buffer, cxStringBody(data), cxStringLength(data));
    }
    if(evhttp_make_request(conn->conn, this->request, EVHTTP_REQ_POST, cxStringBody(suri)) != 0){
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
    cxHttpConn conn = cxHttpGetConnect(this);
    if(conn == NULL){
        CX_ERROR("get http connection error");
        return NULL;
    }
    cxString suri = cxHttpGetUri(this);
    if(suri == NULL){
        CX_ERROR("get uri error");
        return NULL;
    }
    if(evhttp_make_request(conn->conn, this->request, EVHTTP_REQ_GET, cxStringBody(suri)) != 0){
        CX_ERROR("make request error");
        return NULL;
    }
    return this;
}

