//
//  cxActionXML.h
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxActionXML_h
#define cxEngine_cxActionXML_h

#include <core/cxAction.h>
#include "cxEventItem.h"
#include "cxArray.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxActionXML, cxObject)
    cxBool isError;
    cxHash events;
    cxHash codes;
    CX_EVENT_ALLOC(onLoad);
    CX_METHOD_DEF(cxXMLScriptMakeElementFunc, Make);
CX_OBJECT_END(cxActionXML)

cxAny cxActionXMLMakeElement(const xmlChar *temp,xmlTextReaderPtr reader);

cxBool cxActionRootXMLReadAttr(cxAny pxml,cxAny newobj, xmlTextReaderPtr reader);

cxAny cxActionXMLCreate(cxConstChars xml);

cxAny cxActionXMLGet(cxConstChars xml,cxConstChars key);

void cxViewRunActionEvent(cxAny pview,cxAny arg);

void cxPlaySoundEvent(cxAny object,cxAny arg);

void cxPrintMessageEvent(cxAny pview,cxAny arg);

CX_C_END

#endif
