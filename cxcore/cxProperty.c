//
//  cxProperty.c
//  cxCoreIOS
//
//  Created by xuhua on 5/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxProperty.h"

CX_TYPE(cxProperty, cxObject)
{}
CX_INIT(cxProperty, cxObject)
{}
CX_FREE(cxProperty, cxObject)
{}
CX_TERM(cxProperty, cxObject)

void cxRunPropertySetter(cxAny object,cxConstChars key,cxAny value)
{
    CX_ASSERT(key != NULL && object != NULL && value != NULL, "args error");
    cxProperty p = cxObjectProperty(object, key);
    if(p != NULL && p->setter != NULL){
        p->setter(object,value);
    }
}

cxBool cxRunPropertyGetter(cxAny object,cxConstChars key,cxAny *value)
{
    CX_ASSERT(key != NULL && object != NULL, "args error");
    cxProperty p = cxObjectProperty(object, key);
    if(p != NULL && p->getter != NULL){
        *value = p->getter(object);
        return true;
    }
    return false;
}

