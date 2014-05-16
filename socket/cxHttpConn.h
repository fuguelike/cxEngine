//
//  cxHttpConn.h
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxHttpConn_h
#define cxEngine_cxHttpConn_h

#include <event.h>
#include <evhttp.h>
#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxHttpConn, cxObject)
    cxString host;
    cxInt port;
    struct evhttp_connection *conn;
CX_OBJECT_END(cxHttpConn)

cxHttpConn cxHttpConnectOpen(cxConstChars host,cxInt port);

CX_C_END

#endif
