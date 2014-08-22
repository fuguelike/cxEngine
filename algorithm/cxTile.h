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

cxVec2f cxTileVec2iToVec2f(cxVec2i idx,cxSize2f tileSize);

cxVec2i cxTileVec2fToVec2i(cxVec2f pos,cxSize2f tileSize);

CX_C_END

#endif
