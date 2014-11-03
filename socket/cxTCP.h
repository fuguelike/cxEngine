//
//  cxTCP.h
//  cxEngine
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTCP_h
#define cxEngine_cxTCP_h

#include <arpa/inet.h>
#include <unistd.h>
#include <event.h>
#include <cxcore/cxBase.h>

CX_C_BEGIN

#define CX_TCP_BUFFER_SIZE 1024

CX_DEF(cxTCP, cxObject)
    evutil_socket_t socket;
    CX_FIELD_DEF(cxFloat ReadTimeout);
    CX_FIELD_DEF(cxFloat WriteTimeout);
    cxInt port;
    cxString host;
    cxChar buf[CX_TCP_BUFFER_SIZE];
    cxInt bytes;
    CX_FIELD_DEF(cxBool IsConnected);
    CX_EVENT_ALLOC(onRead);
    CX_EVENT_ALLOC(onError);
    CX_EVENT_ALLOC(onClose);
    CX_EVENT_ALLOC(onConnected);
    struct bufferevent *bufferEvent;
    CX_FIELD_DEF(cxAny UserData);
CX_END(cxTCP, cxObject)

CX_FIELD_SET(cxTCP, cxFloat, ReadTimeout);
CX_FIELD_SET(cxTCP, cxFloat, WriteTimeout);
CX_FIELD_GET(cxTCP, cxBool, IsConnected);
CX_FIELD_IMP(cxTCP, cxAny, UserData);

CX_INLINE cxAny cxTcpGetBuffer(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    return this->buf;
}

CX_INLINE cxInt cxTcpGetBytes(cxAny ptcp)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    return this->bytes;
}

cxBool cxTcpWrite(cxAny ptcp,cxString data);

//create new socket connect host
cxInt cxTcpConnect(cxAny ptcp);

void cxTcpClose(cxAny ptcp);

void cxTcpBufferClose(cxAny ptcp);

CX_INLINE void cxTcpSetHost(cxAny ptcp,cxConstChars host)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    CX_RETAIN_SET(this->host, cxStringConstChars(host));
}
CX_INLINE void cxTcpSetPort(cxAny ptcp,cxInt port)
{
    CX_ASSERT_THIS(ptcp, cxTCP);
    this->port = port;
}

cxTCP cxTCPCreate(cxConstChars host,cxInt port);

CX_C_END

#endif
