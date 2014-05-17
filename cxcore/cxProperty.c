//
//  cxProperty.c
//  cxCoreIOS
//
//  Created by xuhua on 5/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxProperty.h"

CX_OBJECT_TYPE(cxProperty, cxObject)
{
    
}
CX_OBJECT_INIT(cxProperty, cxObject)
{
    
}
CX_OBJECT_FREE(cxProperty, cxObject)
{

}
CX_OBJECT_TERM(cxProperty, cxObject)

void cxObjectSetter(cxAny object,cxConstChars key,cxAny value)
{
    CX_ASSERT(key != NULL && object != NULL && value != NULL, "args error");
    cxObject this = object;
    cxType curr = cxTypesGet(this->cxType);
    while(curr != NULL){
        cxProperty p = cxHashGet(curr->properties, cxHashStrKey(key));
        if(p != NULL && p->setter != NULL){
            p->setter(object,value);
            break;
        }
        curr = curr->superType;
    }
}

cxBool cxObjectGetter(cxAny object,cxConstChars key,cxAny *value)
{
    CX_ASSERT(key != NULL && object != NULL, "args error");
    cxObject this = object;
    cxType curr = cxTypesGet(this->cxType);
    while(curr != NULL){
        cxProperty p = cxHashGet(curr->properties, cxHashStrKey(key));
        if(p != NULL && p->getter != NULL){
            *value = p->getter(object);
            return true;
        }
        curr = curr->superType;
    }
    return false;
}

