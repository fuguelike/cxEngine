//
//  cxPolygon.h
//  cxEngine
//
//  Created by xuhua on 1/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxPolygon_h
#define cxEngine_cxPolygon_h

#include "cxSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxPolygon, cxSprite)
    cxVec3f *points;
    cxColor4f *colors;
    cxTex2f *texs;
    cxInt capacity;
    cxInt number;
CX_OBJECT_END(cxPolygon, cxSprite)

void cxPolygonClear(cxAny pview);

void cxPolygonAppend(cxAny pview,cxPoint point);

void cxPolygonResize(cxAny pview,cxInt add);

CX_C_END

#endif
