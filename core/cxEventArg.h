//
//  cxEventArg.h
//  cxEngine
//  use json pack
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEventArg_h
#define cxEngine_cxEventArg_h

#include <json/jansson.h>
#include "cxBase.h"
#include "cxString.h"
#include "cxNumber.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxEventArg, cxObject)
    cxAny weakRef;
    cxAny strongRef;
    json_t *json;
    cxNumber number;
CX_OBJECT_END(cxEventArg)

cxEventArg cxEventArgNumber(cxNumber number);

cxEventArg cxEventArgCreate(cxConstChars json);

cxEventArg cxEventArgStrongRef(cxAny strongRef);

cxAny cxEventArgToStrongRef(cxEventArg this);

cxEventArg cxEventArgWeakRef(cxAny weakRef);

cxAny cxEventArgToWeakRef(cxEventArg this);

cxBool cxEventArgBool(cxEventArg this,cxConstChars key);

cxBool cxEventArgToBool(cxEventArg this);

cxInt cxEventArgInt(cxEventArg this,cxConstChars key);

cxInt cxEventArgToInt(cxEventArg this);

cxConstChars cxEventArgString(cxEventArg this,cxConstChars key);

cxConstChars cxEventArgToString(cxEventArg this);

cxDouble cxEventArgDouble(cxEventArg this,cxConstChars key);

cxDouble cxEventArgToDouble(cxEventArg this);

CX_C_END

#endif
