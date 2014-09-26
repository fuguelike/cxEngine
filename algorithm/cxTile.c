//
//  cxTile.c
//  cxCore
//
//  Created by xuhua on 8/22/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxTile.h"

cxVec2f cxTileIdxToPos(cxVec2f idx,cxSize2f size)
{
    cxFloat w2 = size.w / 2;
    cxFloat h2 = size.h / 2;
    cxFloat a = atan2f(h2, w2);
    cxFloat m = sqrtf(w2*w2 + h2*h2);
    cxFloat dx = m * idx.x;
    cxFloat dy = m * idx.y;
    cxVec2f p1 = cxVec2fv(0, 0);
    p1.x = cosf(a) * dx;
    p1.y = sinf(a) * dx;
    cxVec2f p2 = cxVec2fv(0, 0);
    p2.x = cosf(a) * dy;
    p2.y = sinf(a) * dy;
    return cxVec2fv(p1.x - p2.x,p1.y + p2.y);
}

/*
 计算到达两斜边的平行长度，除边距获取二维索引
 */
cxVec2f cxTilePosToIdx(cxVec2f pos,cxSize2f size)
{
    cxFloat w2 = size.w / 2;
    cxFloat h2 = size.h / 2;
    cxFloat a = atan2f(h2, w2);
    cxFloat m = sqrtf(w2*w2 + h2*h2);
    cxFloat dx = (pos.y / sinf(a) + pos.x / cosf(a)) / 2.0f;
    cxFloat dy = dx - pos.x / cosf(a);
    return cxVec2fv(dx/m, dy/m);
}
