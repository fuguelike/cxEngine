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
    
    cxJsonTypeInit();
    cxIconvTypeInit();
    
    cxStreamTypeInit();
    cxTextureTypeInit();
    
    cxShaderTypeInit();
    cxOpenGLTypeInit();
    
    cxActionTypeInit();
    cxActionSetTypeInit();
    cxTimerTypeInit();
    
    cxViewTypeInit();
    cxWindowTypeInit();
    cxSpriteTypeInit();
    cxViewRootTypeInit();
    cxButtonTypeInit();
    cxLoadingTypeInit();
    cxLabelTTFTypeInit();
    cxParticleTypeInit();
    cxChipmunkTypeInit();
    
    cxEngineTypeInit();
}