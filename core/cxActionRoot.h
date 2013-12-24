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
    cxHash codes;
    CX_EVENT_ALLOC(onLoad);
    CX_METHOD_DEF(cxXMLScriptMakeElementFunc, Make);
CX_OBJECT_END(cxActionRoot)

cxAny cxActionRootMakeElement(cxConstChars temp,xmlTextReaderPtr reader);

void cxActionRootReadAttr(cxReaderAttrInfo *info);

cxAny cxActionRootCreate(cxConstChars xml);

cxAny cxActionRootGet(cxConstChars src);

CX_C_END

#endif
