//
//  cxRunParticle.h
//  cxEngine
//  粒子
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxRunParticle_h
#define cxEngine_cxRunParticle_h

#include <core/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxRunParticle, cxAction)
    cxAny particleView;
    CX_SLOT_ALLOC(onDraw);
CX_OBJECT_END(cxRunParticle)

cxRunParticle cxRunParticleCreateWithPEX(cxConstChars file);

cxRunParticle cxRunParticleCreate(cxAny particleView);

CX_C_END

#endif
