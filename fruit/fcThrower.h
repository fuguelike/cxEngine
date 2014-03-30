//
//  fcThrower.h
//  cxEngine
//  水果发射机
//  Created by xuhua on 3/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcThrower_h
#define fruit_fcThrower_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcThrower, fcSprite)
    //类型
    fcThrowerType type;
    //属性
    cxFloat attackRate;     //攻击频率
    cxFloat attackRange;    //攻击距离
    cxInt attackNumber;     //可攻击的数量,0表示不会攻击
    cxFloat attackPower;    //攻击力
    //水果创建者
    CX_METHOD_ALLOC(cxAny, FruitMaker,cxAny this);
    CX_METHOD_ALLOC(void, FruitFire,cxAny this,cxAny fruit,cxAny target);
CX_OBJECT_END(fcThrower)
//初始化投掷器
void fcThrowerInit(cxAny this,cxAny map,fcThrowerType type);
//开始搜索投掷
void fcThrowerLoop(cxAny this);
CX_C_END

#endif
