//
//  Range.h
//  Home
//
//  Created by xuhua on 9/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Range_h
#define Home_Range_h

#include <views/cxAtlas.h>

CX_C_BEGIN

CX_OBJECT_DEF(Range, cxAtlas)
    cxRange2f value;
CX_OBJECT_END(Range, cxAtlas)

void RangeUpdateValue(cxAny pview);

void RangeSetMin(cxAny pview,cxFloat min);

void RangeSetMax(cxAny pview,cxFloat max);

CX_C_END

#endif
