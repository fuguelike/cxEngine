//
//  cxHttpConn.c
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxLooper.h"
#include "cxHttpConn.h"

CX_TYPE(cxHttpConn, cxObject)
{
    
}
CX_INIT(cxHttpConn, cxObject)
{
    
}
CX_FREE(cxHttpConn, cxObject)
{
    CX_EVENT_RELEASE(this->onClose);
    if(this->conn != NULL){
        evhttp_connection_free(this->conn);
    }
    CX_RELEASE(this->host);
}
CX_TERM(cxHttpConn, cxObject)

static void cxHttpConnClose(struct evhttp_connection *conn, void *pobj)
{
    CX_ASSERT_THIS(pobj, cxHttpConn);
    CX_EVENT_FIRE(this, onClose);
    CX_LOGGER("http connection close");
}

void cxHttpConnSetTimeout(cxAny pthis,cxInt s)
{
    CX_ASSERT_THIS(pthis, cxHttpConn);
    evhttp_connection_set_timeout(this->conn, s);
}

void cxHttpConnSetRetries(cxAny pthis,cxInt s)
{
    CX_ASSERT_THIS(pthis, cxHttpConn);
    evhttp_connection_set_retries(this->conn, s);
}

cxHttpConn cxHttpConnectOpen(cxConstChars host,cxInt port)
{
    CX_ASSERT(host != NULL && port > 0, "args error");
    cxLooper looper = cxLooperInstance();
    cxHttpConn this = CX_CREATE(cxHttpConn);
    this->host = cxStringAllocChars(host);
    this->port = port;
    this->conn = evhttp_connection_base_new(looper->looper, NULL, host, port);
    if(this->conn == NULL){
        CX_ERROR("evhttp new connection error");
        return NULL;
    }
    cxHttpConnSetTimeout(this, 180);
    cxHttpConnSetRetries(this, 3);
    evhttp_connection_set_closecb(this->conn, cxHttpConnClose, this);
    return this;
}

