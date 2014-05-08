//
//  cxInitType);
//  cxEngineIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxUtil.h"
#include "cxInitType.h"

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
    CX_METHOD_RELEASE(this->Create);
    CX_METHOD_RELEASE(this->Alloc);
}
CX_OBJECT_TERM(cxType, cxObject)

void cxInitTypes()
{
    CX_REGISTER_TYPE(cxView);
    CX_REGISTER_TYPE(cxAtlas);
    CX_REGISTER_TYPE(cxButton);
    CX_REGISTER_TYPE(cxClipping);
    CX_REGISTER_TYPE(cxLabelBMP);
    CX_REGISTER_TYPE(cxLabelTTF);
    CX_REGISTER_TYPE(cxLoading);
    CX_REGISTER_TYPE(cxPolygon);
    CX_REGISTER_TYPE(cxScroll);
    CX_REGISTER_TYPE(cxSprite);
    CX_REGISTER_TYPE(cxTable);
    CX_REGISTER_TYPE(cxWindow);
    
    CX_REGISTER_TYPE(cxActionSet);
    CX_REGISTER_TYPE(cxAnimate);
    CX_REGISTER_TYPE(cxFade);
    CX_REGISTER_TYPE(cxFollow);
    CX_REGISTER_TYPE(cxJump);
    CX_REGISTER_TYPE(cxMove);
    CX_REGISTER_TYPE(cxParabola);
    CX_REGISTER_TYPE(cxParticle);
    CX_REGISTER_TYPE(cxRotate);
    CX_REGISTER_TYPE(cxRunner);
    CX_REGISTER_TYPE(cxScale);
    CX_REGISTER_TYPE(cxSpline);
    CX_REGISTER_TYPE(cxTimer);
    CX_REGISTER_TYPE(cxTint);
}








