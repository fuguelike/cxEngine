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

CX_DEF(cxHttp, cxObject)
    CX_FIELD_DEF(cxLong Tag);
    CX_FIELD_DEF(cxBool IsSuccess);
    CX_FIELD_DEF(cxInt64 BodyBytes);
    CX_FIELD_DEF(cxInt64 ReadBytes);
    CX_FIELD_DEF(cxString Data);
    CX_FIELD_DEF(cxString URL);
    cxBool isCancel;
    cxString suri;
    cxChar *pdata;
    cxInt psize;
    struct evhttp_request *request;
    struct evhttp_uri *uri;
    CX_EVENT_ALLOC(onChunked);
    CX_EVENT_ALLOC(onCompleted);
    cxBool autoRelease;
CX_END(cxHttp, cxObject)

CX_FIELD_IMP(cxHttp, cxLong, Tag);
CX_FIELD_GET(cxHttp, cxString, URL);
CX_FIELD_GET(cxHttp, cxString, Data);
CX_FIELD_GET(cxHttp, cxInt64, ReadBytes);
CX_FIELD_GET(cxHttp, cxInt64, BodyBytes);
CX_FIELD_GET(cxHttp, cxBool, IsSuccess);

CX_INLINE void cxHttpSetAuto(cxAny http)
{
    CX_ASSERT_THIS(http, cxHttp);
    CX_RETURN(this->autoRelease);
    CX_RETAIN(this);
    this->autoRelease = true;
}

cxString cxHttpUriEncode(cxString uri);

cxString cxHttpUriDecode(cxString uri);

void cxHttpCancel(cxAny http);

void cxHttpAddHeader(cxAny http,cxString key,cxString value);

cxHttp cxHttpGet(cxString url,cxBool chunked);

cxHttp cxHttpPost(cxString url,cxString data,cxBool chunked);

CX_C_END

#endif
