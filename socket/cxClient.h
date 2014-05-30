//
//  cxClient.h
//  cxEngine
//
//  Created by xuhua on 11/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxClient_h
#define cxEngine_cxClient_h

#include <event.h>
#include <cxcore/cxBase.h>

CX_C_BEGIN

//what value from bufferevent.h
//#define BEV_EVENT_READING	0x01	/**< error encountered while reading */
//#define BEV_EVENT_WRITING	0x02	/**< error encountered while writing */
//#define BEV_EVENT_EOF		0x10	/**< eof file reached */
//#define BEV_EVENT_ERROR		0x20	/**< unrecoverable error encountered */
//#define BEV_EVENT_TIMEOUT	0x40	/**< user-specified timeout reached */
//#define BEV_EVENT_CONNECTED	0x80	/**< connect operation finished. */

CX_OBJECT_DEF(cxClient, cxObject)
    evutil_socket_t socket;
    cxInt port;
    cxString host;
    struct bufferevent *bufevent;
    CX_METHOD_DEF(void, Event,cxClient,cxInt what);
    CX_METHOD_DEF(void, Read,cxClient,struct evbuffer *);
    CX_METHOD_DEF(void, Write,cxClient,struct evbuffer *);
CX_OBJECT_END(cxClient, cxObject)

cxClient cxClientCreate(cxConstChars host,cxInt port);

cxBool cxClientConnect(cxClient this);

cxInt cxClientWriteBytes(cxClient this,cxPointer buffer,cxInt size);

cxInt cxClientWriteString(cxClient this,cxString data);

CX_C_END

#endif
