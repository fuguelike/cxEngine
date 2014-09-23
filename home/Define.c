//
//  Define.c
//  home
//
//  Created by xuhua on 9/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "Define.h"

Global global={0};

//8方向使用 22.5 45.0角度
cxFloat AngleToIndex(cxFloat angle,cxInt *index)
{
    angle = fmodf(kmRadiansToDegrees(angle) + 360, 360);
    cxInt idx = angle/22.5f;
    cxInt m = (idx / 2) + (idx % 2);
    if(m > 7) m = 0;
    if(index != NULL){
        *index = m;
    }
    return (angle - m * 45.0f);
}

void GlobalInit(cxEngine engine)
{
    global.unitNum = cxVec2iv(MAP_ROW, MAP_COL);
    
    cxSize2f size = cxSize2fv(engine->winsize.w * 1.2f, 0);
    size.h = size.w * 0.75f;
    
    global.unitSize = cxSize2fv(size.w/global.unitNum.x, size.h/global.unitNum.y);
    
    cxFloat w2 = global.unitSize.w / 2.0f;
    cxFloat h2 = global.unitSize.h / 2.0f;
    global.sideLen = sqrtf(w2*w2 + h2*h2);
    
    global.angle = asinf(w2 / global.sideLen);
}

cxFloat SideDistance(cxInt sideNum)
{
    return global.sideLen * sideNum;
}