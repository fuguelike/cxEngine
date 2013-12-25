//
//  cxInitType.c
//  cxEngineIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxViewRoot.h"
#include "cxEngine.h"
#include "cxActionRoot.h"
#include "cxUtil.h"
#include "cxInitType.h"

void cxInitTypes()
{
    cxObjectTypeInit();
    cxNumberTypeInit();
    cxStringTypeInit();
    cxUtilTypeInit();
    
    cxHashTypeInit();
    cxArrayTypeInit();
    cxListTypeInit();
    cxStackTypeInit();
    
    cxIconvTypeInit();
    
    cxStreamTypeInit();
    cxPlayerTypeInit();
    cxAssetsStreamTypeInit();
    cxMemStreamTypeInit();
    cxFileStreamTypeInit();
    
    cxTextureFactoryTypeInit();
    cxTexturePKMTypeInit();
    cxTexturePNGTypeInit();
    cxTexturePVRTypeInit();
    cxTextureXMLTypeInit();
    cxTextureTXTTypeInit();
    cxTextureTypeInit();
    
    cxShaderClippingTypeInit();
    cxShaderDefaultTypeInit();
    cxShaderAlphaTypeInit();
    cxShaderPositionColorTypeInit();
    
    cxShaderTypeInit();
    cxOpenGLTypeInit();
    
    cxActionTypeInit();
    cxActionSetTypeInit();
    cxTimerTypeInit();
    cxMoveTypeInit();
    cxActionSetTypeInit();
    cxFadeTypeInit();
    cxJumpTypeInit();
    cxRotateTypeInit();
    cxTintTypeInit();
    cxActionRootTypeInit();
    
    cxViewTypeInit();
    cxClippingTypeInit();
    cxTableTypeInit();
    cxScrollTypeInit();
    cxWindowTypeInit();
    cxSpriteTypeInit();
    cxViewRootTypeInit();
    cxAtlasTypeInit();
    cxButtonTypeInit();
    cxLoadingTypeInit();
    cxLabelTTFTypeInit();
    cxParticleTypeInit();
    cxChipmunkTypeInit();
    
    cxEngineTypeInit();
}