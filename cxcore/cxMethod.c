//
//  cxMethod.c
//  cxCore
//
//  Created by xuhua on 11/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxMethod.h"
#include "cxType.h"

CX_TYPE(cxMethod, cxObject)
{
    //type property regiter
}
CX_INIT(cxMethod, cxObject)
{
    //init data
}
CX_FREE(cxMethod, cxObject)
{
    //free data
}
CX_TERM(cxMethod, cxObject)

cxAny cxMethodGet(cxAny object,cxConstChars key)
{
    CX_RETURN(object == NULL, NULL);
    cxType type = cxObjectType(object);
    CX_RETURN(type == NULL, NULL);
    cxMethod m = cxTypeGetMethod(type, key);
    CX_ASSERT(m != NULL && m->method != NULL, "%s method %d not exists",CX_NAME_OF(object),key);
    return m->method;
}

cxBool cxMethodHas(cxAny object,cxConstChars key)
{
    CX_RETURN(object == NULL, false);
    cxType type = cxObjectType(object);
    CX_RETURN(type == NULL, false);
    cxMethod m = cxTypeGetMethod(type, key);
    return m != NULL && m->method != NULL;
}

cxAny cxMethodSuper(cxAny object,cxConstChars key)
{
    CX_RETURN(object == NULL, NULL);
    cxType type = cxObjectType(object);
    CX_RETURN(type == NULL, NULL);
    cxMethod m = cxTypeGetMethod(type->superType, key);
    CX_ASSERT(m != NULL && m->method != NULL, "%s method %d not exists",CX_NAME_OF(object),key);
    return m->method;
}
