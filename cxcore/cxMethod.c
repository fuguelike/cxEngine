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
    cxMethod m = cxTypeMethod(type, key);
    CX_RETURN(m == NULL, NULL);
    return m->method;
}

cxAny cxMethodSuper(cxAny object,cxConstChars key)
{
    CX_RETURN(object == NULL, NULL);
    cxType type = cxObjectType(object);
    CX_RETURN(type == NULL, NULL);
    cxMethod m = cxTypeMethod(type->superType, key);
    CX_RETURN(m == NULL, NULL);
    return m->method;
}
