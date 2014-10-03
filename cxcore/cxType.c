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
    CX_TYPE_REG(cxObject);
    CX_TYPE_REG(cxType);
    CX_TYPE_REG(cxProperty);
    CX_TYPE_REG(cxMemPool);
    CX_TYPE_REG(cxHash);
    CX_TYPE_REG(cxArray);
    CX_TYPE_REG(cxList);
    CX_TYPE_REG(cxStack);
    CX_TYPE_REG(cxNumber);
    CX_TYPE_REG(cxString);
    CX_TYPE_REG(cxMessage);
    CX_TYPE_REG(cxLoader);
    CX_TYPE_REG(cxPath);
    CX_TYPE_REG(cxStream);
}

void cxTypesFree()
{
    CX_RELEASE(types);
}

cxType cxTypesGet(cxConstType typeName)
{
    return cxHashGet(types, cxHashStrKey(typeName));
}

//replace property if same name
cxProperty cxTypeSetProperty(cxType this,cxConstChars key)
{
    cxHashKey ikey = cxHashStrKey(key);
    cxProperty p = cxHashGet(this->properties, ikey);
    if(p == NULL){
        p = CX_ALLOC(cxProperty);
        cxHashSet(this->properties, ikey, p);
        CX_RELEASE(p);
    }
    return p;
}

cxProperty cxTypeProperty(cxType this,cxConstChars key)
{
    cxProperty p = NULL;
    cxType curr = this;
    cxHashKey ikey = cxHashStrKey(key);
    while (curr != NULL) {
        p = cxHashGet(curr->properties, ikey);
        if(p != NULL){
            break;
        }
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

//a.b.c
void cxTypeSignature(cxType type,cxType super)
{
    CX_ASSERT(type != NULL, "args error");
    if(super == NULL){
        cxStringFormat(type->signature, "%s",cxObjectTypeName);
    }else{
        cxStringFormat(type->signature, "%s.%s",type->typeName,cxStringBody(super->signature));
    }
}

void cxTypeSetSuper(cxType type,cxType super)
{
    CX_ASSERT(type != NULL, "type arsg error");
    CX_RETAIN_SWAP(type->superType, super);
}

cxBool cxObjectInstanceOf(cxAny object,cxConstType type)
{
    cxObject this = object;
    CX_RETURN(object == NULL && type == NULL,true);
    CX_RETURN(object == NULL || type == NULL, false);
    CX_RETURN(this->cxType == type, true);
    CX_RETURN(type == cxObjectTypeName,true);
    cxType ptype = cxTypesGet(this->cxType);
    CX_ASSERT(ptype != NULL, "type %s not register",this->cxType);
    return cxStringHasConstChars(ptype->signature, type);
}

void cxTypeRunObjectSetter(cxObject object,cxJson json)
{
    CX_ASSERT(object != NULL, "object args error");
    CX_JSON_OBJECT_EACH_BEG(json, item)
    cxObjectSetter(object, itemKey, item);
    CX_JSON_OBJECT_EACH_END(json, item)
}

static void cxObjectSave(cxAny object,cxJson json)
{
    cxConstChars cxId = cxJsonConstChars(json, "cxId");
    CX_RETURN(cxId == NULL);
    cxLoader curr = cxCoreTop(cxCoreStackTypeLoader);
    if(curr != NULL){
        CX_ASSERT_TYPE(curr, cxLoader);
        cxHashSet(curr->objects, cxHashStrKey(cxId), object);
    }
}

cxAny cxObjectCreateWithType(cxConstType type)
{
    CX_ASSERT(type != NULL, "type null");
    cxType ptype = cxTypesGet(type);
    CX_ASSERT(ptype != NULL, "type(%s) not register",type);
    return ptype->Create();
}

cxAny cxObjectCreateWithJson(cxJson json)
{
    CX_ASSERT_TYPE(json, cxJson);
    cxAny object = NULL;
    cxJson ojson = json;
    cxJson njson = NULL;
    cxConstChars src = NULL;
    cxConstChars type = NULL;
    if(cxJsonIsString(json)){
        src = cxJsonToConstChars(json);
    }else if(cxJsonIsObject(json)){
        src = cxJsonConstChars(json, "cxSrc");
    }
    //from src get json
    if(src != NULL){
        njson = cxJsonRead(src);
        CX_ASSERT(njson != NULL, "read json failed from %s",src);
        type = cxJsonConstChars(njson, "cxType");
    }else{
        type = cxJsonConstChars(ojson, "cxType");
    }
    CX_ASSERT(type != NULL, "json cxType property null");
    object = cxObjectCreateWithType(type);
    CX_ASSERT(object != NULL,"create object %s failed", type);
    //read new json property
    if(njson != NULL){
        cxTypeRunObjectSetter(object, njson);
        cxObjectSave(object, njson);
    }
    //read old json property
    if(cxJsonIsObject(ojson)){
        cxTypeRunObjectSetter(object, ojson);
        cxObjectSave(object, ojson);
    }
    return object;
}

CX_OBJECT_TYPE(cxType, cxObject)
{
    
}
CX_OBJECT_INIT(cxType, cxObject)
{
    this->properties = CX_ALLOC(cxHash);
    this->signature = CX_ALLOC(cxString);
}
CX_OBJECT_FREE(cxType, cxObject)
{
    CX_RELEASE(this->signature);
    CX_RELEASE(this->properties);
    CX_RELEASE(this->superType);
}
CX_OBJECT_TERM(cxType, cxObject)









