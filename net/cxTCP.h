//
//  cxTCP.h
//  cxEngine
//
//  Created by xuhua on 11/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTCP_h
#define cxEngine_cxTCP_h

#include <uv/uv.h>
#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_DEF(cxTCP, cxObject)
    CX_FIELD_DEF(cxBool IsConnected);
    CX_FIELD_DEF(cxString Host);
    CX_FIELD_DEF(cxInt Port);
    uv_tcp_t handle;
    uv_connect_t connreq;
    uv_getaddrinfo_t resolver;
    struct addrinfo hints;
    cxAny buffer;
    cxInt bufsiz;
    CX_FIELD_DEF(cxInt Error);
CX_END(cxTCP, cxObject)
CX_FIELD_GET(cxTCP, cxInt, Error);
CX_FIELD_GET(cxTCP, cxBool, IsConnected);
CX_FIELD_IMP(cxTCP, cxInt, Port);
CX_FIELD_IMO(cxTCP, cxString, Host);

//release in close
void cxTCPClose(cxAny ptcp,cxInt error);

cxBool cxTCPWrite(cxAny ptcp,const cxString data);

cxBool cxTCPConnect(cxAny ptcp);

CX_C_END

#endif
