//
//  cxViewXML.h
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxViewXML_h
#define cxEngine_cxViewXML_h

#include <core/cxView.h>
#include <core/cxEventItem.h>
#include <core/cxEventArg.h>
#include <streams/cxAssetsStream.h>
#include <textures/cxTextureFactory.h>
#include <views/cxSprite.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxViewXML, cxView)
    cxBool isError;
    cxHash items;
    cxHash events;
    cxHash actions;
    CX_METHOD_DEF(cxXMLScriptMakeElementFunc, Make);
    CX_EVENT_ALLOC(onBegin);
    CX_EVENT_ALLOC(onEnd);
CX_OBJECT_END(cxViewXML)

void cxViewXMLSetItem(cxAny pview,cxConstChars key,cxAny item);

cxAny cxViewXMLMakeElement(const xmlChar *temp,xmlTextReaderPtr reader);

cxBool cxViewRootXMLReadAttr(cxAny pxml,cxAny view, xmlTextReaderPtr reader);

void cxViewXMLRegisteEvent(cxAny pview,cxConstChars name,cxEventFunc func);

cxViewXML cxViewXMLCreate(cxConstChars xml);

void cxViewXMLRemove(cxAny pview,cxConstChars key);

cxAny cxViewXMLGet(cxAny pview,cxConstChars key);

void cxViewReplaceViewEvent(cxEvent *event);

void cxViewPushViewEvent(cxEvent *event);

void cxViewPopViewEvent(cxEvent *event);

cxBool cxViewXMLLoad(cxAny pview,cxConstChars xml);

cxBool cxViewXMLLoadWithReader(cxAny pview,xmlTextReaderPtr reader);

CX_C_END

#endif







