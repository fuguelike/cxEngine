//
//  cxActionRoot.h
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxActionRoot_h
#define cxEngine_cxActionRoot_h

#include <core/cxAction.h>
#include "cxEventItem.h"
#include "cxArray.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxActionRoot, cxObject)
    cxBool isError;
    cxHash events;
    cxHash functions;
    cxHash codes;
    CX_EVENT_ALLOC(onLoad);
    CX_METHOD_DEF(cxXMLScriptMakeElementFunc, Make);
CX_OBJECT_END(cxActionRoot)

void cxActionRootRegisteEvent(cxAny pview,cxConstChars name,cxEventFunc func);

void cxActionRootRegisteFunc(cxAny pview,cxConstChars name,cxAny func);

cxAny cxActionRootMakeElement(cxConstChars temp,xmlTextReaderPtr reader);

void cxActionRootReadAttr(cxAny pxml,cxAny newobj, xmlTextReaderPtr reader);

cxAny cxActionRootCreate(cxConstChars xml);

cxAny cxActionRootGet(cxConstChars src);

void cxViewRunActionEvent(cxEvent *event);

void cxPlaySoundEvent(cxEvent *event);

void cxPrintMessageEvent(cxEvent *event);

CX_C_END

#endif
