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

/*
 计算到达两斜边的平行长度，除边距获取二维索引
 */
cxVec2i cxTileVec2fToVec2i(cxVec2f pos,cxSize2f size)
{
    cxVec2i idx = cxVec2iv(0, 0);
    //格子半宽高
    cxFloat w2 = size.w / 2;
    cxFloat h2 = size.h / 2;
    //边长倍数
    cxInt d = 0;
    //边长
    cxFloat m = sqrtf(w2*w2 + h2*h2);
    //斜率
    cxFloat k = h2 / w2;
    //获取idx.y 距离方程
    //y = kx - h2;
    //y2—y1=k(X2—X1
    cxVec2f p1 = cxVec2fv(0, 0);
    p1.x = (k * pos.x + pos.y + h2)/ (2 * k);
    p1.y = k * p1.x - h2;
    d = kmVec2DistanceBetween(&p1, &pos) / m;
    idx.y = (pos.y < (k * pos.x - h2))?(-d-1) : d;
    //获取idx.x 距离方程
    //y = -kx - h2;
    cxVec2f p2 = cxVec2fv(0, 0);
    p2.x = (k * pos.x - pos.y - h2) / (2 * k);
    p2.y = -k * p2.x - h2;
    d = kmVec2DistanceBetween(&p2, &pos) / m;
    idx.x = (pos.y < (-k * pos.x - h2))?(-d-1) : d;
    return idx;
}
