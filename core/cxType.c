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

void cxTypeInit()
{
    types = CX_ALLOC(cxHash);
}

void cxTypeFree()
{
    CX_RELEASE(types);
}

cxType cxTypeGet(cxConstType typeName)
{
    return cxHashGet(types, cxHashStrKey(typeName));
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
    //all NULL type
    CX_RETURN(object == NULL && type == NULL,true);
    //
    CX_RETURN(object == NULL, false);
    cxObject this = object;
    if(this->cxType == type){
        return true;
    }
    cxType ptype = cxTypeGet(this->cxType);
    while (ptype != NULL && ptype->superType != NULL) {
        if(ptype->superType->typeName == type){
            return true;
        }
        ptype = ptype->superType;
    }
    return false;
}

cxAny cxTypeCreate(cxJson json,cxHash hash)
{
    CX_ASSERT(json != NULL, "json error");
    cxConstChars type = cxJsonConstChars(json, "type");
    cxType ptype = cxTypeGet(type);
    CX_ASSERT(ptype != NULL, "type(%s) not register",type);
    cxObject object = CX_METHOD_GET(NULL, ptype->Create);
    CX_ASSERT(object != NULL, "type(%s) create object failed",type);
    CX_METHOD_RUN(object->cxInit,object,json,hash);
    return object;
}

CX_OBJECT_INIT(cxType, cxObject)
{
    
}
CX_OBJECT_FREE(cxType, cxObject)
{
    CX_RELEASE(this->superType);
}
CX_OBJECT_TERM(cxType, cxObject)

void cxInitTypes()
{
    //register object class
    cxType tmp = CX_ALLOC(cxType);
    CX_METHOD_SET(tmp->Alloc, __cxObjectAllocFunc);
    CX_METHOD_SET(tmp->Create, __cxObjectCreateFunc);
    cxTypeSet(cxObjectTypeName,tmp);
    CX_RELEASE(tmp);
    
    //register core
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
}

//parse type id subview property
cxAny cxObjectLoadByJson(cxJson json, cxHash hash)
{
    CX_ASSERT(json != NULL, "json args error");
    //link to src file
    cxConstChars src = cxJsonConstChars(json, "src");
    cxObject object = NULL;
    if(src != NULL){
        object = cxObjectLoad(src, hash);
        CX_METHOD_RUN(object->cxInit,object,json,hash);
    }else {
        object = cxTypeCreate(json,hash);
    }
    CX_RETURN(object == NULL, NULL);
    //save view to hash
    cxConstChars id = cxJsonConstChars(json, "id");
    if(id != NULL && hash != NULL && cxHashSet(hash, cxHashStrKey(id), object)){
        CX_WARN("view id %s exists in hash");
    }
    return object;
}
//save to hash with id
cxAny cxObjectLoad(cxConstChars file,cxHash hash)
{
    cxUrlPath path = cxUrlPathParse(file);
    cxJson json = cxEngineLoadJsonFile(path->path);
    CX_RETURN(json == NULL, NULL);
    //get file.json?key
    if(path->count == 2){
        json = cxJsonObject(json, path->key);
    }
    CX_RETURN(json == NULL, NULL);
    return cxObjectLoadByJson(json, hash);
}








