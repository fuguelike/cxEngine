//
//  cxHTTP.c
//  cxEngine
//
//  Created by xuhua on 12/9/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//


#include "cxHTTP.h"

//http://www.baidu.com:80/a/b/x.php?aa=221
static void cxParseURL(char *url, char *server, int *port, char *path)
{
    int serverlen = 0;
    int numread = 0;
    url = url+7;
    sscanf(url, "%255[^/:]", server);
    serverlen = (int)strlen(server);
    if(url[serverlen] == ':'){
        sscanf(&url[serverlen+1], "%d%n", port, &numread);
        numread++;
    }else{
        *port = 80;
    }
    strcpy(path, &url[serverlen+numread]);
}

//when onconnected
CX_METHOD_DEF(cxHTTP, OnConnected, void)
{
    this->offset = 0;
    cxHTTPSetReqHeader(this, "Host", cxTCPGetHost(this));
    cxStr s = CX_CREATE(cxStr);
    if(this->Method == HTTP_GET){
        cxStrFormat(s, "GET %s HTTP/1.1\r\n",cxStrBody(this->Path));
    }else if(this->Method == HTTP_POST){
        cxStrFormat(s, "POST %s HTTP/1.1\r\n",cxStrBody(this->Path));
    }
    if(cxStrOK(this->Post)){
        cxHTTPSetReqHeader(this, "Content-Length", UTF8("%d",cxStrLength(this->Post)));
    }
    CX_HASH_FOREACH(this->reqHeads, ele, tmp){
        cxStrFormat(s, "%s: %s\r\n",ele->key,cxStrBody(ele->any));
    }
    cxStrFormat(s, "\r\n");
    //write head
    if(!cxTCPWrite(this, s)){
        CX_ERROR("write tcp data error");
        return;
    }
    //write body
    if(this->Method == HTTP_POST && cxStrOK(this->Post)){
        cxTCPWrite(this, this->Post);
    }
}
//when closed
CX_METHOD_DEF(cxHTTP, OnClose, void)
{
    CX_EVENT_FIRE(this, onClose);
}
//when read data
CX_METHOD_DEF(cxHTTP, OnRead, void, cxAny buffer,cxInt size)
{
    cxStrAppend(this->data, buffer, size);
    cxAny buf = cxStrBody(this->data) + this->offset;
    cxInt siz = cxStrLength(this->data) - this->offset;
    this->offset = (cxInt)http_parser_execute(&this->parser, &this->settins, buf, (size_t)siz);
}
CX_METHOD_DEF(cxHTTP, OnCompleted, void)
{
    CX_EVENT_FIRE(this, onCompleted);
}
CX_METHOD_DEF(cxHTTP, OnHeader, void)
{
    
}
CX_METHOD_DEF(cxHTTP, OnStart, void)
{
    
}
static int cxHTTPOnBodyFunc(http_parser *parser, const char *at, size_t length)
{
    CX_ASSERT_THIS(parser->data, cxHTTP);
    cxStrAppend(this->Body, at, (cxInt)length);
    return 0;
}
static int cxHTTPOnHeadField(http_parser *parser, const char *at, size_t length)
{
    CX_ASSERT_THIS(parser->data, cxHTTP);
    cxStrAppend(this->field, at, (cxInt)length);
    return 0;
}
static int cxHTTPOnHeadValue(http_parser *parser, const char *at, size_t length)
{
    CX_ASSERT_THIS(parser->data, cxHTTP);
    cxConstChars key = cxStrBody(this->field);
    cxHashSet(this->resHeads, cxHashStrKey(key), cxStrBinary((cxAny)at, (cxInt)length));
    cxStrClear(this->field);
    return 0;
}
static int cxHTTPMessageBegin(http_parser *parser)
{
    CX_ASSERT_THIS(parser->data, cxHTTP);
    CX_CALL(this, OnStart, CX_M(void));
    return 0;
}
static int cxHTTPHeadCompleted(http_parser *parser)
{
    CX_ASSERT_THIS(parser->data, cxHTTP);
    CX_CALL(this, OnHeader, CX_M(void));
    return 0;
}
static int cxHTTPMessageCompleted(http_parser *parser)
{
    CX_ASSERT_THIS(parser->data, cxHTTP);
    this->IsSuccess = parser->status_code == 200;
    CX_CALL(this, OnCompleted, CX_M(void));
    return 0;
}
CX_TYPE(cxHTTP, cxTCP)
{
    CX_METHOD(cxHTTP, OnClose);
    CX_METHOD(cxHTTP, OnRead);
    CX_METHOD(cxHTTP, OnConnected);
    //
    CX_METHOD(cxHTTP, OnCompleted);
    CX_METHOD(cxHTTP, OnHeader);
    CX_METHOD(cxHTTP, OnStart);
}
CX_INIT(cxHTTP, cxTCP)
{
    this->IsSuccess = false;
    this->parser.data = this;
    http_parser_init(&this->parser, HTTP_RESPONSE);
    this->settins.on_body = cxHTTPOnBodyFunc;
    this->settins.on_header_field = cxHTTPOnHeadField;
    this->settins.on_header_value = cxHTTPOnHeadValue;
    this->settins.on_message_begin = cxHTTPMessageBegin;
    this->settins.on_headers_complete = cxHTTPHeadCompleted;
    this->settins.on_message_complete = cxHTTPMessageCompleted;
    this->Body = CX_ALLOC(cxStr);
    this->data = CX_ALLOC(cxStr);
    this->field = CX_ALLOC(cxStr);
    this->reqHeads = CX_ALLOC(cxHash);
    this->resHeads = CX_ALLOC(cxHash);
    
    cxHTTPSetReqHeader(this, "User-Agent", UTF8("Mozilla/4.0(compatible;MSIE6.0;Windows NT 5.0)"));
    cxHTTPSetReqHeader(this, "Connection", UTF8("close"));
    cxHTTPSetMethod(this, HTTP_GET);
    
}
CX_FREE(cxHTTP, cxTCP)
{
    CX_RELEASE(this->field);
    CX_RELEASE(this->data);
    CX_RELEASE(this->resHeads);
    CX_RELEASE(this->reqHeads);
    CX_RELEASE(this->Body);
    CX_RELEASE(this->Path);
    CX_RELEASE(this->Post);
    CX_EVENT_RELEASE(this->onCompleted);
    CX_EVENT_RELEASE(this->onClose);
}
CX_TERM(cxHTTP, cxTCP)

void cxHTTPSetReqHeader(cxAny phttp,cxConstChars key,cxStr value)
{
    CX_ASSERT_THIS(phttp, cxHTTP);
    cxHashSet(this->reqHeads, cxHashStrKey(key), value);
}

static cxBool cxHTTPInitURL(cxAny phttp,cxStr url)
{
    CX_ASSERT_THIS(phttp, cxHTTP);
    cxChar server[255]={0};
    int port;
    cxChar path[1024]={0};
    cxParseURL(cxStrBody(url), server, &port, path);
    if(strlen(server) <= 0){
        return false;
    }
    cxTCPSetPort(this, port);
    cxTCPSetHost(this, cxStrConstChars(server));
    //set path
    cxHTTPSetPath(this, strlen(path)>0?cxStrConstChars(path):UTF8("/"));
    return true;
}

cxBool cxHTTPInitGet(cxAny phttp,cxStr url)
{
    CX_ASSERT_THIS(phttp, cxHTTP);
    if(!cxHTTPInitURL(this, url)){
        CX_ERROR("init url %s error",cxStrBody(url));
        return false;
    }
    return cxTCPConnect(this);
}

cxHTTP cxHTTPPost(cxStr url,cxStr post)
{
    CX_ASSERT(cxStrOK(url), "args url error");
    cxHTTP this = CX_ALLOC(cxHTTP);
    cxHTTPSetMethod(this, HTTP_POST);
    cxHTTPSetPost(this, post);
    if(!cxHTTPInitGet(this, url)){
        CX_RELEASE(this);
        return NULL;
    }
    return this;
}

//release on close
cxHTTP cxHTTPGet(cxStr url)
{
    CX_ASSERT(cxStrOK(url), "args url error");
    cxHTTP this = CX_ALLOC(cxHTTP);
    cxHTTPSetMethod(this, HTTP_GET);
    if(!cxHTTPInitGet(this, url)){
        CX_RELEASE(this);
        return NULL;
    }
    return this;
}

