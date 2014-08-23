//
//  cxTile.c
//  cxCore
//
//  Created by xuhua on 8/22/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxTile.h"

cxVec2f cxTileVec2iToVec2f(cxVec2i idx,cxSize2f size)
{
    cxFloat w2 = size.w / 2;
    cxFloat h2 = size.h / 2;
    cxFloat x = idx.x * w2 - idx.y * w2;
    cxFloat y = idx.x * h2 + idx.y * h2;
    return cxVec2fv(x,y);
}

cxVec2i cxTileVec2fToVec2i(cxVec2f pos,cxSize2f size)
{
    cxFloat w2 = size.w / 2;
    cxFloat h2 = size.h / 2;
    //边长
    cxFloat m = sqrtf(w2*w2 + h2*h2);
    //斜率
    cxFloat k = h2 / w2;
    //获取idx.y 距离方程
    //y = kx - h2;
    //y2—y1=k(X2—X1
    cxVec2f p = cxVec2fv(0, 0);
    p.x = (k * pos.x + pos.y + h2)/ (2 * k);
    p.y = k * p.x - h2;
    cxInt dy = kmVec2DistanceBetween(&p, &pos) / m;
    //获取idx.x 距离方程
    //y = -kx - h2;
    p.x = (k * pos.x - pos.y - h2) / (2 * k);
    p.y = -k * p.x - h2;
    cxInt dx = kmVec2DistanceBetween(&p, &pos) / m;
    return cxVec2iv(dx,dy);
}