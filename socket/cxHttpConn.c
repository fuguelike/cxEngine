//
//  cxHttpConn.c
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEventBase.h"
#include "cxHttpConn.h"

CX_OBJECT_INIT(cxHttpConn, cxObject)
{
    
}
CX_OBJECT_FREE(cxHttpConn, cxObject)
{
    if(this->conn != NULL){
        evhttp_connection_free(this->conn);
    }
    CX_RELEASE(this->host);
}
CX_OBJECT_TERM(cxHttpConn, cxObject)

cxHttpConn cxHttpConnectOpen(cxConstChars host,cxInt port)
{
    CX_ASSERT(host != NULL && port > 0, "args error");
    cxEventBase base= cxEventBaseInstance();
    cxHttpConn this = CX_CREATE(cxHttpConn);
    this->host = cxStringAllocChars(host);
    this->port = port;
    this->conn = evhttp_connection_base_new(base->base, NULL, host, port);
    if(this->conn == NULL){
        CX_ERROR("evhttp new connection error");
        return NULL;
    }
    return this;
}

