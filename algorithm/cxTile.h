//
//  cxTile.h
//  cxCore
//
//  Created by xuhua on 8/22/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxTile_h
#define cxCore_cxTile_h

#include <engine/cxMath.h>

CX_C_BEGIN

cxVec2f cxTileIdxToPos(cxVec2f idx,cxSize2f size);

cxVec2f cxTilePosToIdx(cxVec2f pos,cxSize2f size);

CX_C_END

#endif
