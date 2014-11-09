//
//  cxHttp.c
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxLooper.h"
#include "cxHttpConn.h"
#include "cxHttp.h"

cxString cxHttpUriEncode(cxString uri)
{
    CX_RETURN(uri == NULL, NULL);
    cxChars data = evhttp_uriencode(cxStringBody(uri), cxStringLength(uri), 0);
    CX_RETURN(data == NULL, NULL);
    return cxStringAttachMem(data, (cxInt)strlen(data));
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
    this->ReadBytes = this->request->body_size;
    size_t len = evbuffer_get_length(this->request->input_buffer);
    if(len <= 0){
        return;
    }
    if(len > this->psize){
        this->pdata = allocator->realloc(this->pdata,len);
        this->psize = (cxInt)len;
    }
    cxInt bytes = (cxInt)evbuffer_copyout(this->request->input_buffer, this->pdata, len);
    if(bytes > 0){
        cxStringAppend(this->Data, this->pdata, bytes);
    }
}

static void cxHttpRequestCompleted(struct evhttp_request *req,void *xhttp)
{
    CX_ASSERT_THIS(xhttp, cxHttp);
    cxHttpReadData(this);
    CX_EVENT_FIRE(this, onCompleted);
    if(this->release){
        CX_AUTO_RELEASE(this);
    }
}

static void cxHttpRequestChunked(struct evhttp_request *req,void *xhttp)
{
    CX_ASSERT_THIS(xhttp, cxHttp);
    cxStringClear(this->Data);
    cxHttpReadData(this);
    CX_EVENT_FIRE(this, onChunked);
    if(this->isCancel){
        evhttp_cancel_request(req);
    }
}

static int cxHttpRequestHeader(struct evhttp_request *req,void *xhttp)
{
    CX_ASSERT_THIS(xhttp, cxHttp);
    this->IsSuccess = this->request->response_code == HTTP_OK;
    if(!this->IsSuccess){
        CX_ERROR("http request error:%d",this->request->response_code);
        return -1;
    }
    if(this->BodyBytes == 0 && this->request->headers_size > 0){
        cxConstChars cl = evhttp_find_header(this->request->input_headers, "Content-Length");
        this->BodyBytes = cl != NULL ? atoll(cl) : 0;
    }
    return 0;
}

static void cxHttpRequestError(enum evhttp_request_error error, void *xhttp)
{
    CX_ASSERT_THIS(xhttp, cxHttp);
    this->error = error;
    CX_EVENT_FIRE(this, onError);
    CX_ERROR("http request error:%d",error);
}

//url
CX_SETTER_DEF(cxHttp, url)
{
    cxString url = cxJsonString(value, "url");
    CX_ASSERT(cxStringOK(url), "url error");
    cxHttpSetURL(this, url);
}
//method
CX_SETTER_DEF(cxHttp, method)
{
    cxConstChars method = cxJsonConstChars(value, "method");
    if(cxConstCharsEqu(method, "POST")){
        cxHttpSetMethod(this, cxHttpMethodPost);
    }else if(cxConstCharsEqu(method, "GET")){
        cxHttpSetMethod(this, cxHttpMethodGet);
    }else{
        cxHttpSetMethod(this, cxHttpMethodGet);
    }
}
//data
CX_SETTER_DEF(cxHttp, data)
{
    cxString data = cxJsonString(value, "data");
    CX_RETURN(!cxStringOK(data));
    cxHttpSetPostData(this, data);
}
//chunked
CX_SETTER_DEF(cxHttp, chunked)
{
    cxBool chunked = cxJsonBool(value, "chunked", false);
    cxHttpSetChunked(this, chunked);
}
//autorelease at completed
CX_SETTER_DEF(cxHttp, release)
{
    if(cxJsonBool(value, "release", false)){
        cxHttpSetRelease(this);
    }
}
CX_TYPE(cxHttp, cxObject)
{
    CX_SETTER(cxHttp, url);
    CX_SETTER(cxHttp, method);
    CX_SETTER(cxHttp, data);
    CX_SETTER(cxHttp, chunked);
    CX_SETTER(cxHttp, release);
}
CX_INIT(cxHttp, cxObject)
{
    this->Data = CX_ALLOC(cxString);
    this->request = evhttp_request_new(cxHttpRequestCompleted, this);
    evhttp_request_set_error_cb(this->request, cxHttpRequestError);
    evhttp_request_set_header_cb(this->request, cxHttpRequestHeader);
    this->request->flags |= EVHTTP_REQ_DEFER_FREE;
}
CX_FREE(cxHttp, cxObject)
{
    if(this->uri != NULL){
        evhttp_uri_free(this->uri);
    }
    if(this->pdata != NULL){
        allocator->free(this->pdata);
    }
    if(this->request != NULL){
        evhttp_request_free(this->request);
    }
    CX_EVENT_RELEASE(this->onError);
    CX_EVENT_RELEASE(this->onCompleted);
    CX_EVENT_RELEASE(this->onChunked);
    CX_RELEASE(this->URL);
    CX_RELEASE(this->Data);
    CX_RELEASE(this->suri);
}
CX_TERM(cxHttp, cxObject)

static cxHttpConn cxHttpGetConnect(cxAny http)
{
    CX_ASSERT_THIS(http, cxHttp);
    cxConstChars host = evhttp_uri_get_host(this->uri);
    cxInt port = evhttp_uri_get_port(this->uri);
    return cxLooperHttpConnect(host,port < 0 ? 80 : port);
}

void cxHttpCancel(cxAny http)
{
    CX_ASSERT_THIS(http, cxHttp);
    this->isCancel = true;
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

void cxHttpSetChunked(cxAny pthis,cxBool chunked)
{
    CX_ASSERT_THIS(pthis, cxHttp);
    evhttp_request_set_chunked_cb(this->request, chunked? cxHttpRequestChunked : NULL);
}

cxBool cxHttpSetURL(cxAny phttp,cxString url)
{
    CX_ASSERT_THIS(phttp, cxHttp);
    if(!cxStringOK(url)){
        return false;
    }
    //parse uri from url
    CX_RETAIN_SWAP(this->URL, url);
    this->uri = evhttp_uri_parse(cxStringBody(url));
    if(this->uri == NULL){
        CX_ERROR("http parse uri error");
        return false;
    }
    //set uri
    cxString suri = cxHttpGetUri(this);
    if(suri == NULL){
        CX_ERROR("http get uri error");
        return false;
    }
    CX_RETAIN_SWAP(this->suri, suri);
    //set host
    evhttp_add_header(this->request->output_headers, "Host", evhttp_uri_get_host(this->uri));
    return true;
}

void cxHttpAddHeader(cxAny phttp,cxString key,cxString value)
{
    CX_ASSERT_THIS(phttp, cxHttp);
    CX_ASSERT(cxStringOK(key) && cxStringOK(value), "error key or value");
    evhttp_add_header(this->request->output_headers, cxStringBody(key), cxStringBody(value));
}

void cxHttpSetPostData(cxAny phttp,cxString data)
{
    CX_ASSERT_THIS(phttp, cxHttp);
    if(!cxStringOK(data)){
        return;
    }
    evbuffer_add(this->request->output_buffer, cxStringBody(data), cxStringLength(data));
}

cxBool cxHttpSetMethod(cxAny phttp,cxHttpMethod type)
{
    CX_ASSERT_THIS(phttp, cxHttp);
    cxHttpConn conn = cxHttpGetConnect(this);
    if(conn == NULL){
        CX_ERROR("get http connection error");
        return false;
    }
    if(evhttp_make_request(conn->conn, this->request, (enum evhttp_cmd_type)type, cxStringBody(this->suri)) != 0){
        CX_ERROR("make request error");
        return false;
    }
    return true;
}

cxHttp cxHttpPost(cxString url,cxString data,cxBool chunked)
{
    cxHttp this = CX_CREATE(cxHttp);
    if(!cxHttpSetURL(this,url)){
        CX_ERROR("cx http init error");
        return NULL;
    }
    cxHttpSetChunked(this, chunked);
    cxHttpSetPostData(this, data);
    if(!cxHttpSetMethod(this, cxHttpMethodPost)){
        return NULL;
    }
    return this;
}

cxHttp cxHttpGet(cxString url,cxBool chunked)
{
    cxHttp this = CX_CREATE(cxHttp);
    if(!cxHttpSetURL(this,url)){
        CX_ERROR("cx http init error");
        return NULL;
    }
    cxHttpSetChunked(this, chunked);
    if(!cxHttpSetMethod(this, cxHttpMethodGet)){
        return NULL;
    }
    return this;
}

