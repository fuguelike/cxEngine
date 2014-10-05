//
//  cxSprites.h
//  cxCore
//  draw multiple sprite use atlas
//  Created by xuhua on 10/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSprites_h
#define cxEngine_cxSprites_h

#include "cxAtlas.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxSprites, cxAtlas)
    cxArray sprites;
CX_OBJECT_END(cxSprites, cxAtlas)

CX_C_END

#endif
