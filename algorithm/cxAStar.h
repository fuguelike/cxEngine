//
//  cxAStar.h
//  cxEngine
//
//  Created by xuhua on 1/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAStar_h
#define cxEngine_cxAStar_h

#include <core/cxBase.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxAStar, cxObject)
    cxVec2i max;
    cxVec2i src;
    cxVec2i dst;
CX_OBJECT_END(cxAStar)

cxBool cxAStarRun(cxAStar this,cxVec2i src,cxVec2i dst);

cxAStar cxAStarCreate(cxVec2i max);

CX_C_END

#endif
