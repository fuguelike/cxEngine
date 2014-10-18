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

#define CX_SPINE_MAX_VERTICES (SP_VERTEX_Y4 + 1)

CX_DEF(cxSpine, cxAtlas)
    spAtlas *atlas;
    spSkeleton *skeleton;
    spSkeletonData* skeletonData;
    spBone *rootBone;
    spAnimationStateData *stateData;
    cxFloat vertices[CX_SPINE_MAX_VERTICES];
CX_END(cxSpine, cxAtlas)

void cxSpineSetMixs(cxAny pview,cxJson mixs);

cxBool cxSpineInit(cxAny pview, cxConstChars atlas,cxConstChars json,cxJson mixs,cxFloat scale);

CX_C_END

#endif
