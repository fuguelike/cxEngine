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
    CX_ASSERT(index != NULL, "args error");
    cxFloat degrees = kmRadiansToDegrees(angle) + 360.0f;
    angle = fmodf(degrees, 360.0f);
    cxInt idx = angle/22.5f;
    cxInt m = (idx / 2) + (idx % 2);
    if(m > 7) m = 0;
    *index = m;
    return (angle - m * 45.0f);
}

void GlobalInit(cxEngine engine)
{
    global.mapRate = 1.5f;
    global.unitNum = cxVec2iv(MAP_ROW, MAP_COL);
    
    global.mapSize = cxSize2fv(engine->DesSize.w * global.mapRate, 0);
    global.mapSize.h = global.mapSize.w * 0.75f;
    
    global.unitSize.w = global.mapSize.w/global.unitNum.x;
    global.unitSize.h = global.mapSize.h/global.unitNum.y;
    
    cxFloat w2 = global.unitSize.w / 2.0f;
    cxFloat h2 = global.unitSize.h / 2.0f;
    global.sideLen = sqrtf(w2*w2 + h2*h2);
    
    global.angle = asinf(w2 / global.sideLen);
    
    global.warUnitNum = cxVec2iv(32, 18);
    global.warUnitSize = cxSize2fv(512, 512);
    global.warMapSize.w = global.warUnitSize.w * global.warUnitNum.x;
    global.warMapSize.h = global.warUnitSize.h * global.warUnitNum.y;
    
    global.warShowNum.x = (engine->DesSize.w / global.warUnitSize.w  + 4)/2 ;
    global.warShowNum.y = (engine->DesSize.h / global.warUnitSize.h  + 4)/2;
}

cxFloat SideDistance(cxInt sideNum)
{
    return global.sideLen * sideNum;
}