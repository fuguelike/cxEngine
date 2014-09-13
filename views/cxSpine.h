//
//  cxSpine.h
//  cxEngine
//
//  Created by xuhua on 9/13/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSpine_h
#define cxEngine_cxSpine_h

#include <spine/spine.h>
#include "cxAtlas.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxSpine, cxAtlas)
    spAtlas *atlas;
    spSkeleton *skeleton;
    spSkeletonData* skeletonData;
    spBone *rootBone;
    cxFloat *worldVertices;
    spAnimationState *state;
CX_OBJECT_END(cxSpine, cxAtlas)

CX_C_END

#endif
