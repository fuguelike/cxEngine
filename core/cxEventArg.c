//
//  cxEventArg.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxEventArg.h"

CX_OBJECT_INIT(cxEventItem, cxObject)
{
    
}
CX_OBJECT_FREE(cxEventItem, cxObject)
{
    CX_RELEASE(this->arg);
}
CX_OBJECT_TERM(cxEventItem, cxObject)

CX_OBJECT_INIT(cxEventArg, cxObject)
{
    
}
CX_OBJECT_FREE(cxEventArg, cxObject)
{
    this->weakRef = NULL;
    CX_RELEASE(this->strongRef);
    CX_RELEASE(this->json);
    CX_RELEASE(this->number);
}
CX_OBJECT_TERM(cxEventArg, cxObject)

cxAny cxEventArgToWeakRef(cxEventArg this)
{
    CX_RETURN(this == NULL,NULL);
    return this->weakRef;
}

cxEventArg cxEventArgWeakRef(cxAny weakRef)
{
    cxEventArg this = CX_CREATE(cxEventArg);
    this->weakRef = weakRef;
    return this;
}

cxAny cxEventArgToStrongRef(cxEventArg this)
{
    CX_RETURN(this == NULL,NULL);
    return this->strongRef;
}

cxEventArg cxEventArgStrongRef(cxAny strongRef)
{
    cxEventArg this = CX_CREATE(cxEventArg);
    CX_RETAIN_SWAP(this->strongRef, strongRef);
    return this;
}

cxEventArg cxEventArgCreateWithNumber(cxConstChars json,cxNumber number)
{
    cxEventArg this = cxEventArgCreate(json);
    cxEventArgSetNumber(this, number);
    return this;
}

cxEventArg cxEventArgCreate(cxConstChars json)
{
    CX_RETURN(json == NULL,NULL);
    cxEventArg this = CX_CREATE(cxEventArg);
    this->json = cxJsonCreate(UTF8(json));
    CX_RETAIN(this->json);
    return this;
}

cxNumber cxEventArgNumber(cxEventArg this)
{
    return this->number;
}

void cxEventArgSetNumber(cxEventArg this,cxNumber num)
{
    CX_RETAIN_SWAP(this->number, num);
}

cxBool cxEventArgToBool(cxEventArg this,cxBool dv)
{
    CX_ASSERT(this->json != NULL, "args error");
    return cxJsonToBool(this->json, dv);
}

cxBool cxEventArgBool(cxEventArg this,cxConstChars key,cxBool dv)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    return cxJsonBool(this->json, key, dv);
}

cxInt cxEventArgToInt(cxEventArg this,cxInt dv)
{
    CX_ASSERT(this->json != NULL, "args error");
    return cxJsonToInt(this->json, dv);
}

cxLong cxEventArgLong(cxEventArg this,cxConstChars key,cxLong dv)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    return cxJsonLong(this->json, key, dv);
}

cxLong cxEventArgToLong(cxEventArg this,cxLong dv)
{
    CX_ASSERT(this->json != NULL, "args error");
    return cxJsonToLong(this->json, dv);
}

cxInt cxEventArgInt(cxEventArg this,cxConstChars key,cxInt dv)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    return cxJsonInt(this->json, key, dv);
}

cxDouble cxEventArgToDouble(cxEventArg this,cxDouble dv)
{
    CX_ASSERT(this->json != NULL, "args error");
    return cxJsonToDouble(this->json, dv);
}

cxDouble cxEventArgDouble(cxEventArg this,cxConstChars key,cxDouble dv)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    return cxJsonDouble(this->json, key, dv);
}

cxConstChars cxEventArgToString(cxEventArg this)
{
    CX_ASSERT(this->json != NULL, "args error");
    return cxJsonToString(this->json);
}

cxConstChars cxEventArgString(cxEventArg this,cxConstChars key)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    return cxJsonString(this->json, key);
}







