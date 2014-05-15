//
//  cxInitType);
//  cxEngineIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxUtil.h"
#include "cxType.h"

#include "cxShader.h"
#include "cxOpenGL.h"
#include "cxStream.h"
#include "cxPlayer.h"
#include "cxTexture.h"
#include "cxIconv.h"

#include <textures/cxTextureFactory.h>
#include <textures/cxTextureJPG.h>
#include <textures/cxTexturePKM.h>
#include <textures/cxTexturePNG.h>
#include <textures/cxTexturePVR.h>
#include <textures/cxTextureTXT.h>
#include <textures/cxTextureJSON.h>

#include <shaders/cxShaderAlpha.h>
#include <shaders/cxShaderDefault.h>
#include <shaders/cxShaderPositionColor.h>
#include <shaders/cxShaderClipping.h>

#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <streams/cxMemStream.h>

#include <views/cxButton.h>
#include <views/cxLoading.h>
#include <views/cxLabelTTF.h>
#include <views/cxWindow.h>
#include <views/cxScroll.h>
#include <views/cxTable.h>
#include <views/cxClipping.h>
#include <views/cxLabelBMP.h>
#include <views/cxPolygon.h>

#include <actions/cxParticle.h>
#include <actions/cxActionSet.h>
#include <actions/cxMove.h>
#include <actions/cxFade.h>
#include <actions/cxJump.h>
#include <actions/cxRotate.h>
#include <actions/cxTimer.h>
#include <actions/cxTint.h>
#include <actions/cxFollow.h>
#include <actions/cxAnimate.h>
#include <actions/cxParabola.h>
#include <actions/cxRunner.h>
#include <actions/cxScale.h>
#include <actions/cxSpline.h>

static cxHash types;

static void cxInitTypes()
{
    //register object class
    cxType tmp = CX_ALLOC(cxType);
    CX_METHOD_SET(tmp->Alloc, __cxObjectAllocFunc);
    CX_METHOD_SET(tmp->Create, __cxObjectCreateFunc);
    cxTypeSet(cxObjectTypeName,tmp);
    __cxObjectInitType(tmp);
    CX_RELEASE(tmp);
    
    //register core
    CX_REGISTER_TYPE(cxType,         cxObject);
    CX_REGISTER_TYPE(cxHash,         cxObject);
    CX_REGISTER_TYPE(cxArray,        cxObject);
    CX_REGISTER_TYPE(cxList,         cxObject);
    CX_REGISTER_TYPE(cxStack,        cxObject);
    CX_REGISTER_TYPE(cxStream,       cxObject);
    CX_REGISTER_TYPE(cxString,       cxObject);
    CX_REGISTER_TYPE(cxTexture,      cxObject);
    CX_REGISTER_TYPE(cxNumber,       cxObject);
    
    //register streams
    CX_REGISTER_TYPE(cxAssetsStream, cxStream);
    CX_REGISTER_TYPE(cxFileStream,   cxStream);
    CX_REGISTER_TYPE(cxMemStream,    cxStream);
    
    //register textures
    CX_REGISTER_TYPE(cxTextureJPG,   cxTexture);
    CX_REGISTER_TYPE(cxTexturePKM,   cxTexture);
    CX_REGISTER_TYPE(cxTexturePNG,   cxTexture);
    CX_REGISTER_TYPE(cxTextureTXT,   cxTexture);
    CX_REGISTER_TYPE(cxTexturePVR,   cxTexture);
    CX_REGISTER_TYPE(cxTextureJSON,  cxTexture);
    
    //register views
    CX_REGISTER_TYPE(cxView,         cxObject);
    CX_REGISTER_TYPE(cxSprite,       cxView);
    CX_REGISTER_TYPE(cxScroll,       cxView);
    CX_REGISTER_TYPE(cxTable,        cxView);
    CX_REGISTER_TYPE(cxWindow,       cxView);
    CX_REGISTER_TYPE(cxClipping,     cxView);
    CX_REGISTER_TYPE(cxLoading,      cxView);
    CX_REGISTER_TYPE(cxPolygon,      cxSprite);
    CX_REGISTER_TYPE(cxAtlas,        cxSprite);
    CX_REGISTER_TYPE(cxButton,       cxSprite);
    CX_REGISTER_TYPE(cxLabelTTF,     cxSprite);
    CX_REGISTER_TYPE(cxLabelBMP,     cxAtlas);
    
    //register actions
    CX_REGISTER_TYPE(cxAction,       cxObject);
    CX_REGISTER_TYPE(cxActionSet,    cxAction);
    CX_REGISTER_TYPE(cxAnimate,      cxAction);
    CX_REGISTER_TYPE(cxFade,         cxAction);
    CX_REGISTER_TYPE(cxFollow,       cxAction);
    CX_REGISTER_TYPE(cxJump,         cxAction);
    CX_REGISTER_TYPE(cxMove,         cxAction);
    CX_REGISTER_TYPE(cxParabola,     cxAction);
    CX_REGISTER_TYPE(cxParticle,     cxAction);
    CX_REGISTER_TYPE(cxRotate,       cxAction);
    CX_REGISTER_TYPE(cxRunner,       cxAction);
    CX_REGISTER_TYPE(cxScale,        cxAction);
    CX_REGISTER_TYPE(cxSpline,       cxAction);
    CX_REGISTER_TYPE(cxTimer,        cxAction);
    CX_REGISTER_TYPE(cxTint,         cxAction);
    
    //register
}

void cxTypeInit()
{
    types = CX_ALLOC(cxHash);
    cxInitTypes();
}

void cxTypeFree()
{
    CX_RELEASE(types);
}

cxType cxTypeGet(cxConstType typeName)
{
    return cxHashGet(types, cxHashStrKey(typeName));
}

cxProperty cxTypeProperty(cxType this,cxConstChars key)
{
    cxAny p = cxHashGet(this->properties, cxHashStrKey(key));
    if(p == NULL){
        p = CX_ALLOC(cxProperty);
        cxHashSet(this->properties, cxHashStrKey(key), p);
        CX_RELEASE(p);
    }
    return p;
}

void cxTypeSet(cxConstType typeName,cxType type)
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
    cxType ptype = cxTypeGet(this->cxType);
    while (ptype != NULL && ptype->superType != NULL) {
        if(ptype->superType->typeName == type){
            return true;
        }
        ptype = ptype->superType;
    }
    return false;
}

void cxTypeInvokeSetter(cxType this,cxConstChars key,cxAny object,cxAny value)
{
    CX_ASSERT(this != NULL && key != NULL && object != NULL && value != NULL, "args error");
    cxType curr = this;
    while(curr != NULL){
        cxProperty p = cxHashGet(this->properties, cxHashStrKey(key));
        if(p != NULL && p->setter != NULL){
            p->setter(object,value);
            break;
        }
        curr = curr->superType;
    }
}

cxAny cxTypeInvokeGetter(cxType this,cxConstChars key,cxAny object)
{
    CX_ASSERT(this != NULL && key != NULL && object != NULL, "args error");
    cxType curr = this;
    while(curr != NULL){
        cxProperty p = cxHashGet(this->properties, cxHashStrKey(key));
        if(p != NULL && p->getter != NULL){
            return p->getter(object);
        }
        curr = curr->superType;
    }
    return NULL;
}

void cxTypeRunObjectSetter(cxObject object,cxJson json)
{
    cxType this = cxTypeGet(object->cxType);
    CX_JSON_OBJECT_EACH_BEG(json, item)
    cxTypeInvokeSetter(this, key, object, item);
    CX_JSON_OBJECT_EACH_END(json, item)
}

cxAny cxTypeCreate(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    cxConstChars type = cxJsonConstChars(json, "type");
    CX_ASSERT(type != NULL, "type field null");
    cxType ptype = cxTypeGet(type);
    CX_ASSERT(ptype != NULL, "type(%s) not register",type);
    cxObject object = CX_METHOD_GET(NULL, ptype->Create);
    CX_ASSERT(object != NULL, "type(%s) create object failed",type);
    cxTypeRunObjectSetter(object, json);
    return object;
}

void __cxTypeInitType(cxAny type)
{
    
}

void __cxTypeInitObject(cxAny object,cxAny json,cxAny hash)
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

//parse type id subview property
cxAny cxObjectLoadWithJson(cxJson json)
{
    CX_ASSERT(json != NULL, "json args error");
    cxConstChars src = NULL;
    cxObject object = NULL;
    //link to src file
    if(cxJsonIsString(json)){
        src = cxJsonToConstChars(json);
    }else if(cxJsonIsObject(json)){
        src = cxJsonConstChars(json, "src");
    }
    if(src != NULL){
        object = cxObjectLoadWithFile(src);
        cxTypeRunObjectSetter(object, json);
    }else {
        object = cxTypeCreate(json);
    }
    if(object == NULL){
        CX_ERROR("create object failed");
        return NULL;
    }
    return object;
}

//save to hash with id
cxAny cxObjectLoadWithFile(cxConstChars file)
{
    cxUrlPath path = cxUrlPathParse(file);
    cxJson json = cxEngineLoadJson(path->path);
    CX_RETURN(json == NULL, NULL);
    //get file.json?key
    if(path->count == 2){
        json = cxJsonObject(json, path->key);
    }
    CX_RETURN(json == NULL, NULL);
    return cxObjectLoadWithJson(json);
}








