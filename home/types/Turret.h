//
//  Turret.h
//  Home
//  炮塔，防御单位
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Turret_h
#define Home_Turret_h

#include <Node.h>

CX_C_BEGIN

CX_OBJECT_DEF(Turret, Node)
    cxInt attackNum;        //同时攻击数量
CX_OBJECT_END(Turret, Node)

void TurretSearch(cxAny pview);

void TurretAttack(cxAny pview);

Turret TurretCreate(cxAny map,cxSize2f size,cxVec2f pos);

CX_C_END

#endif
