//
//  cxChipmunk.h
//  cxEngine
//
//  Created by xuhua on 11/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxChipmunk_h
#define cxEngine_cxChipmunk_h

#include <core/cxView.h>
#include <chipmunk/chipmunk.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxChipmunk, cxView)
    cpSpace *space;
CX_OBJECT_END(cxChipmunk)

void cxChipmunkSetGravity(cxAny pview,cxVec2f gravity);

CX_C_END

#endif
