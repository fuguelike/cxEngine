//
//  fcLocation.h
//  cxEngine
//  起点和终点
//  起点是可以放置兵力的地点，终点是需要盗打的地点，防守方可以自由设置
//  只能在空位置设置，并且亮点之间必须连通
//  Created by xuhua on 4/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcLocation_h
#define fruit_fcLocation_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcLocation, fcSprite)
    fcLocationType type;
CX_OBJECT_END(fcLocation)

void fcLocationInit(cxAny this,cxAny map,fcLocationType type);

CX_C_END

#endif
