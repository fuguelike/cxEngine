//
//  cxHttp.h
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxHttp_h
#define cxEngine_cxHttp_h

#include <evhttp.h>
#include "cxHttpConn.h"

CX_C_BEGIN

typedef void (*cxHttpFunc)(cxAny http);

CX_OBJECT_DEF(cxHttp, cxObject)
    cxBool isSuccess;
    cxInt64 bodyBytes;
    cxInt64 readBytes;
    cxString data;
    cxString suri;
    struct evhttp_request *request;
    struct evhttp_uri *uri;
    CX_EVENT_ALLOC(onChunked);
    CX_EVENT_ALLOC(onCompleted);
CX_OBJECT_END(cxHttp, cxObject)

cxString cxHttpUriEncode(cxString uri);

cxString cxHttpUriDecode(cxString uri);

cxInt cxHttpLength(cxAny http);

void cxHttpCancel(cxAny http);

void cxHttpAddHeader(cxAny http,cxString key,cxString value);

cxString cxHttpBody(cxAny http);

cxHttp cxHttpGet(cxConstChars url,cxBool chunked);

cxHttp cxHttpPost(cxConstChars url,cxString data,cxBool chunked);

CX_C_END

#endif
