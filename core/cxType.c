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

CX_OBJECT_INIT(cxType, cxObject)
{
    
}
CX_OBJECT_FREE(cxType, cxObject)
{
    CX_RELEASE(this->superType);
    CX_METHOD_RELEASE(this->Create);
    CX_METHOD_RELEASE(this->Alloc);
}
CX_OBJECT_TERM(cxType, cxObject)

void cxInitTypes()
{
    //register object class
    cxType objectType = CX_ALLOC(cxType);
    CX_METHOD_OVERRIDE(objectType->Alloc, __cxObjectAllocFunc);
    CX_METHOD_OVERRIDE(objectType->Create, __cxObjectCreateFunc);
    cxEngineSetType(cxObjectTypeName,objectType);
    CX_RELEASE(objectType);
    
    //register views
    CX_REGISTER_TYPE(cxView, cxObject);
    CX_REGISTER_TYPE(cxSprite, cxView);
    CX_REGISTER_TYPE(cxScroll, cxView);
    CX_REGISTER_TYPE(cxTable, cxView);
    CX_REGISTER_TYPE(cxWindow, cxView);
    CX_REGISTER_TYPE(cxClipping, cxView);
    CX_REGISTER_TYPE(cxLoading, cxView);
    CX_REGISTER_TYPE(cxPolygon, cxSprite);
    CX_REGISTER_TYPE(cxAtlas, cxSprite);
    CX_REGISTER_TYPE(cxButton, cxSprite);
    CX_REGISTER_TYPE(cxLabelTTF, cxSprite);
    CX_REGISTER_TYPE(cxLabelBMP, cxAtlas);
    
    //register actions
    CX_REGISTER_TYPE(cxAction, cxObject);
    CX_REGISTER_TYPE(cxActionSet, cxAction);
    CX_REGISTER_TYPE(cxAnimate, cxAction);
    CX_REGISTER_TYPE(cxFade, cxAction);
    CX_REGISTER_TYPE(cxFollow, cxAction);
    CX_REGISTER_TYPE(cxJump, cxAction);
    CX_REGISTER_TYPE(cxMove, cxAction);
    CX_REGISTER_TYPE(cxParabola, cxAction);
    CX_REGISTER_TYPE(cxParticle, cxAction);
    CX_REGISTER_TYPE(cxRotate, cxAction);
    CX_REGISTER_TYPE(cxRunner, cxAction);
    CX_REGISTER_TYPE(cxScale, cxAction);
    CX_REGISTER_TYPE(cxSpline, cxAction);
    CX_REGISTER_TYPE(cxTimer, cxAction);
    CX_REGISTER_TYPE(cxTint, cxAction);
}








