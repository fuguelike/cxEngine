//
//  Soldier.h
//  Home
//  基本近战攻击单位
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Soldier_h
#define Home_Soldier_h

#include <Node.h>

CX_C_BEGIN

CX_OBJECT_DEF(Soldier, Node)
    cxInt attackNum;        //同时攻击数量
CX_OBJECT_END(Soldier, Node)

Soldier SoldierCreate(cxAny map, cxSize2f size,cxVec2f pos);

CX_C_END

#endif
