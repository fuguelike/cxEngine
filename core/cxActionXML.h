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

typedef enum {
    cxActionXMLTypeNone,        //none
    cxActionXMLTypeMultiple,    //multiple
    cxActionXMLTypeSequence,    //sequence
}cxActionXMLType;

CX_OBJECT_DEF(cxActionXML, cxAction)
    cxActionXMLType type;
    cxArray items;
    cxInt index;
    cxBool isError;
    cxHash actions;
    cxHash events;
    CX_METHOD_DEF(cxXMLScriptMakeElementFunc, Make);
    CX_EVENT_ALLOC(onLoad);
CX_OBJECT_END(cxActionXML)

cxAny cxActionXMLMakeElement(const xmlChar *temp,xmlTextReaderPtr reader);

void cxActionXMLRunNext(cxAny pav);

void cxActionXMLRunAll(cxAny pav);

cxAction cxActionXMLAttachView(cxAny pview,cxConstChars xml,cxConstChars key);

cxBool cxActionXMLLoadWithReader(cxAny pav,xmlTextReaderPtr reader);

cxBool cxActionXMLLoad(cxAny pav,cxConstChars xml);

cxAny cxActionXMLGet(cxConstChars xml,cxConstChars name);

cxAny cxActionXMLCreate(cxConstChars xml);

void cxActionXMLSet(cxAny xmlAction,cxAny mAction,xmlTextReaderPtr reader);

void cxViewRunActionEvent(cxAny pview,cxAny arg);

void cxActionRemoveViewEvent(cxAny pav,cxAny arg);

void cxPlaySoundEvent(cxAny object,cxAny arg);

void cxPrintMessageEvent(cxAny pview,cxAny arg);

CX_C_END

#endif
