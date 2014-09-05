//
//  cxInitType);
//  cxCoreIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxBase.h"
#include "cxType.h"

static cxHash types;

void cxTypesInit()
{
    types = CX_ALLOC(cxHash);
    CX_TYPE_DEF(cxObject);
    CX_TYPE_DEF(cxType);
    CX_TYPE_DEF(cxProperty);
    CX_TYPE_DEF(cxMemPool);
    CX_TYPE_DEF(cxHash);
    CX_TYPE_DEF(cxArray);
    CX_TYPE_DEF(cxList);
    CX_TYPE_DEF(cxStack);
    CX_TYPE_DEF(cxNumber);
    CX_TYPE_DEF(cxString);
    CX_TYPE_DEF(cxMessage);
    CX_TYPE_DEF(cxLoader);
}

void cxTypesFree()
{
    CX_RELEASE(types);
}

cxType cxTypesGet(cxConstType typeName)
{
    return cxHashGet(types, cxHashStrKey(typeName));
}

cxProperty cxTypeSetProperty(cxType this,cxConstChars key)
{
    cxProperty p = CX_ALLOC(cxProperty);
    cxHashSet(this->properties, cxHashStrKey(key), p);
    CX_RELEASE(p);
    return p;
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
    CX_RETURN(type == cxObjectTypeName,true);
    cxType ptype = cxTypesGet(this->cxType);
    while (ptype != NULL && ptype->superType != NULL) {
        if(ptype->superType->typeName == type){
            return true;
        }
        ptype = ptype->superType;
    }
    return false;
}

void cxTypeRunObjectSetter(cxObject object,cxJson json)
{
    CX_ASSERT(object != NULL, "object args error");
    CX_JSON_OBJECT_EACH_BEG(json, item)
    cxObjectSetter(object, key, item);
    CX_JSON_OBJECT_EACH_END(json, item)
}

void cxObjectCreateEnd(cxObjectCreateResult *ret)
{
    CX_ASSERT(ret != NULL, "ret null");
    cxConstChars id = NULL;
    cxObject object = ret->object;
    if(ret->njson != NULL){
        id = cxJsonConstChars(ret->njson, "id");
        cxTypeRunObjectSetter(object, ret->njson);
    }
    if(ret->njson != ret->ojson){
        id = cxJsonConstChars(ret->ojson, "id");
        cxTypeRunObjectSetter(object, ret->ojson);
    }
    cxLoader curr = cxCoreTop();
    if(id != NULL && CX_INSTANCE_OF(curr, cxLoader)){
        cxHashSet(curr->objects, cxHashStrKey(id), object);
    }
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

cxObjectCreateResult cxObjectCreateBegin(cxJson json)
{
    CX_ASSERT(json != NULL, "json args error");
    cxObjectCreateResult ret = {NULL};
    ret.ojson = json;
    cxConstChars src = NULL;
    if(cxJsonIsString(json)){
        src = cxJsonToConstChars(json);
    }else if(cxJsonIsObject(json)){
        src = cxJsonConstChars(json, "src");
    }
    //from src get json
    cxConstChars type = NULL;
    if(src != NULL){
        CX_ASSERT(cxJsonReader !=NULL, "please set cxJsonReader");
        ret.njson = cxJsonReader(src);
        CX_ASSERT(ret.njson != NULL, "read json failed from %s",src);
        type = cxJsonConstChars(ret.njson, "type");
    }else{
        type = cxJsonConstChars(ret.ojson, "type");
    }
    CX_ASSERT(type != NULL, "json type property null");
    ret.object = cxObjectCreateWithType(type);
    CX_ASSERT(ret.object != NULL,"create object %s failed", type);
    return ret;
}

void __cxTypeRegisterName(cxConstType type,cxConstType super)
{
    
}

CX_OBJECT_TYPE(cxType, cxObject)
{
    
}
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









