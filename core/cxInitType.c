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
    __cxObjectTypeInit();
    __cxNumberTypeInit();
    __cxStringTypeInit();
    __cxUtilTypeInit();
    
    __cxHashTypeInit();
    __cxArrayTypeInit();
    __cxListTypeInit();
    __cxStackTypeInit();
    
    __cxJsonTypeInit();
    __cxIconvTypeInit();
    
    __cxStreamTypeInit();
    __cxTextureTypeInit();
    
    __cxShaderTypeInit();
    __cxOpenGLTypeInit();
    
    __cxActionTypeInit();
    __cxActionSetTypeInit();
    __cxTimerTypeInit();
    __cxRunParticleTypeInit();
    
    __cxPlayerTypeInit();
    
    __cxViewTypeInit();
    __cxWindowTypeInit();
    __cxSpriteTypeInit();
    __cxViewRootTypeInit();
    __cxButtonTypeInit();
    __cxLoadingTypeInit();
    __cxLabelTTFTypeInit();
    __cxParticleTypeInit();
    __cxScrollTypeInit();
    __cxTableTypeInit();
    
    __cxEngineTypeInit();
}