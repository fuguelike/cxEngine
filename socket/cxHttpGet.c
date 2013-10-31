//
//  cxHttpGet.c
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxHttpGet.h"

CX_OBJECT_INIT(cxHttpGet, cxHttp)
{
   
}
CX_OBJECT_FREE(cxHttpGet, cxHttp)
{
    
}
CX_OBJECT_TERM(cxHttpGet, cxHttp)

cxHttp cxHttpGetCreate(cxConstChars url,cxBool chunked)
{
    cxHttpGet this = CX_CREATE(cxHttpGet);
    if(!cxHttpInit(this,url,chunked)){
        CX_ERROR("cx http init error");
        return NULL;
    }
    cxHttpConnect conn = cxHttpGetConnect(this);
    if(conn == NULL){
        CX_ERROR("get http connection error");
        return NULL;
    }
    cxString suri = cxHttpGetUri(this);
    if(suri == NULL){
        CX_ERROR("get uri error");
        return NULL;
    }
    if(evhttp_make_request(conn->conn, this->super.request, EVHTTP_REQ_GET, cxStringBody(suri)) != 0){
        CX_ERROR("make request error");
        return NULL;
    }
    return (cxHttp)this;
}












