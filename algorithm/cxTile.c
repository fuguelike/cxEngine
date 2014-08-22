//
//  cxTile.c
//  cxCore
//
//  Created by xuhua on 8/22/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxTile.h"

cxVec2f cxTileVec2iToVec2f(cxVec2i idx,cxSize2f tileSize)
{
    cxFloat x = idx.x * tileSize.w/2 - idx.y * tileSize.w/2;
    cxFloat y = idx.x * tileSize.h/2 + idx.y * tileSize.h/2;
    return cxVec2fv(x,y);
}

cxVec2i cxTileVec2fToVec2i(cxVec2f pos,cxSize2f tileSize)
{
    return cxVec2iv(0,0);
}