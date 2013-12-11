//
//  cxEventArg.h
//  cxEngine
//  use json pack
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEventArg_h
#define cxEngine_cxEventArg_h

#include <jansson.h>
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

void cxEventArgSetNumber(cxEventArg this,cxNumber num);

cxEventArg cxEventArgNumber(cxNumber number);

cxEventArg cxEventArgCreateWithNumber(cxConstChars json,cxNumber number);

cxEventArg cxEventArgCreate(cxConstChars json);

cxEventArg cxEventArgStrongRef(cxAny strongRef);

cxAny cxEventArgToStrongRef(cxEventArg this);

cxEventArg cxEventArgWeakRef(cxAny weakRef);

cxAny cxEventArgToWeakRef(cxEventArg this);

cxBool cxEventArgBool(cxEventArg this,cxConstChars key,cxBool dv);

cxBool cxEventArgToBool(cxEventArg this,cxBool dv);

cxInt cxEventArgInt(cxEventArg this,cxConstChars key,cxInt dv);

cxInt cxEventArgToInt(cxEventArg this,cxInt dv);

cxConstChars cxEventArgString(cxEventArg this,cxConstChars key);

cxConstChars cxEventArgToString(cxEventArg this);

cxDouble cxEventArgDouble(cxEventArg this,cxConstChars key,cxDouble dv);

cxDouble cxEventArgToDouble(cxEventArg this,cxDouble dv);

CX_C_END

#endif
