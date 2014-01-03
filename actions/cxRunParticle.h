//
//  cxRunParticle.h
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxRunParticle_h
#define cxEngine_cxRunParticle_h

#include <core/cxAction.h>

CX_C_BEGIN

typedef enum {
    cxRunParticleTypeNone,
    cxRunParticleTypeDraw,
    cxRunParticleTypeAfter,
    cxRunParticleTypeBefore
}cxRunParticleType;

CX_OBJECT_DEF(cxRunParticle, cxAction)
    cxRunParticleType type;
    cxAny particleView;
    CX_SLOT_ALLOC(DrawParticle);
CX_OBJECT_END(cxRunParticle)

cxRunParticle cxRunParticleCreateWithPEX(cxConstChars file,cxRunParticleType type);

cxRunParticle cxRunParticleCreate(cxAny particleView, cxRunParticleType type);

CX_C_END

#endif
