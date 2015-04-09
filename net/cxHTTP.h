//
//  cxHTTP.h
//  cxEngineStatic
//
//  Created by xuhua on 12/9/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxHTTP_h
#define cxEngine_cxHTTP_h

#include "http_parser.h"
#include "cxTCP.h"

CX_C_BEGIN

typedef enum http_method cxHttpMethod;


CX_DEF(cxHTTP, cxTCP)
    CX_FIELD_DEF(cxHttpMethod Method);
    http_parser parser;
    http_parser_settings settins;
    CX_FIELD_DEF(cxStr Path);
    CX_FIELD_DEF(cxStr Body);
    CX_FIELD_DEF(cxStr Post);
    cxHash reqHeads;
    cxHash resHeads;
    cxStr data;
    cxInt offset;
    cxStr field;
    CX_EVENT_ALLOC(onCompleted);
    CX_EVENT_ALLOC(onClose);
    CX_FIELD_DEF(cxBool IsSuccess);
CX_END(cxHTTP, cxTCP)

CX_FIELD_IMP(cxHTTP, cxHttpMethod, Method);
CX_FIELD_GET(cxHTTP, cxBool, IsSuccess);
CX_FIELD_IMO(cxHTTP, cxStr, Path);
CX_FIELD_IMO(cxHTTP, cxStr, Body);
CX_FIELD_IMO(cxHTTP, cxStr, Post);

void cxHTTPSetReqHeader(cxAny phttp,cxConstChars key,cxStr value);

cxHTTP cxHTTPPost(cxStr url,cxStr post);

cxHTTP cxHTTPGet(cxStr url);

CX_C_END

#endif
