//
//  cxHttpConnect.h
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxHttpConnect_h
#define cxEngine_cxHttpConnect_h

#include <event.h>
#include <evhttp.h>
#include <core/cxBase.h>
#include <core/cxString.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxHttpConnect, cxObject)
    cxString host;
    cxInt port;
    struct evhttp_connection *conn;
CX_OBJECT_END(cxHttpConnect)

cxHttpConnect cxHttpConnectOpen(cxConstChars host,cxInt port);

CX_C_END

#endif
