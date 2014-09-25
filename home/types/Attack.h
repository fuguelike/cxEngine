//
//  Attack.h
//  Home
//  主动攻击单位，可以移动
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Attack_h
#define Home_Attack_h

#include <Node.h>

CX_C_BEGIN

CX_OBJECT_DEF(Attack, Node)
    //同时攻击数量
    cxInt attackNum;
    //移动方向变化时
    cxInt index;                //当前的角度索引
    CX_EVENT_ALLOC(onIndex);
    //攻击目标时
    cxAny target;               //当前攻击的目标
    CX_EVENT_ALLOC(onAttack);
CX_OBJECT_END(Attack, Node)

Attack AttackCreate(cxAny map, cxSize2f size,cxVec2f pos);

CX_C_END

#endif
