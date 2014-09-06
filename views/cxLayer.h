//
//  cxLayer.h
//  Engine
//  use one texture,big number effect sprites
//  Created by xuhua on 9/6/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLayer_h
#define cxEngine_cxLayer_h

#include <views/cxAtlas.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxLayer, cxAtlas)
    CX_SLOT_ALLOC(onRemove);
    CX_SLOT_ALLOC(onAppend);
CX_OBJECT_END(cxLayer, cxAtlas)


CX_C_END

#endif
