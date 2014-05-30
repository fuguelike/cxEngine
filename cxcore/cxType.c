//
//  cxInitType);
//  cxCoreIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxType.h"

static cxHash types;

static void cxInitTypes()
{
    CX_REGISTER_TYPE(cxObject);
    //register core
    CX_REGISTER_TYPE(cxType);
    CX_REGISTER_TYPE(cxProperty);
    CX_REGISTER_TYPE(cxMemPool);
    CX_REGISTER_TYPE(cxHash);
    CX_REGISTER_TYPE(cxArray);
    CX_REGISTER_TYPE(cxList);
    CX_REGISTER_TYPE(cxStack);
    CX_REGISTER_TYPE(cxNumber);
    CX_REGISTER_TYPE(cxString);
    CX_REGISTER_TYPE(cxMessage);
}

void cxTypesInit()
{
    types = CX_ALLOC(cxHash);
    cxInitTypes();
}

void cxTypesFree()
{
    CX_RELEASE(types);
}

cxType cxTypesGet(cxConstType typeName)
{
    return cxHashGet(types, cxHashStrKey(typeName));
}

cxProperty cxTypeProperty(cxType this,cxConstChars key)
{
    cxProperty p = NULL;
    cxType curr = this;
    while (curr != NULL) {
        p = cxHashGet(curr->properties, cxHashStrKey(key));
        CX_BREAK(p != NULL);
        curr = curr->superType;
    }
    if(p == NULL){
        p = CX_ALLOC(cxProperty);
        cxHashSet(this->properties, cxHashStrKey(key), p);
        CX_RELEASE(p);
    }
    return p;
}

void cxTypesSet(cxConstType typeName,cxType type)
{
    CX_ASSERT(typeName != NULL && type != NULL, "args error");
    type->typeName = typeName;
    cxHashSet(types, cxHashStrKey(typeName), type);
}

void cxTypeSetSuper(cxType type,cxType super)
{
    CX_ASSERT(type != NULL, "type arsg error");
    CX_RETAIN_SWAP(type->superType, super);
}

cxBool cxInstanceOf(cxAny object,cxConstType type)
{
    cxObject this = object;
    CX_RETURN(object == NULL && type == NULL,true);
    CX_RETURN(object == NULL || type == NULL, false);
    CX_RETURN(this->cxType == type, true);
    cxType ptype = cxTypesGet(this->cxType);
    while (ptype != NULL && ptype->superType != NULL) {
        if(ptype->superType->typeName == type){
            return true;
        }
        ptype = ptype->superType;
    }
    return false;
}

cxAny cxObjectCreateWithType(cxConstType type)
{
    CX_ASSERT(type != NULL, "type null");
    cxType ptype = cxTypesGet(type);
    CX_ASSERT(ptype != NULL, "type(%s) not register",type);
    cxObject object = ptype->Create();
    CX_ASSERT(object != NULL, "type(%s) create object failed",type);
    return object;
}

CX_OBJECT_TYPE(cxType, cxObject)
{}
CX_OBJECT_INIT(cxType, cxObject)
{
    this->properties = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxType, cxObject)
{
    CX_RELEASE(this->properties);
    CX_RELEASE(this->superType);
}
CX_OBJECT_TERM(cxType, cxObject)









