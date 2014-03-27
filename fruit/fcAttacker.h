//
//  fcAttacker.h
//  cxEngine
//
//  Created by xuhua on 3/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcAttacker_h
#define fruit_fcAttacker_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcAttacker, fcSprite)
    //搜索攻击Timer
    cxTimer loopTimer;
    //攻击类型
    fcAttackerType attackerType;
    //属性
    cxFloat attackRate;     //攻击频率
    cxFloat attackRange;    //攻击距离
    cxInt attackNumber;     //可攻击的数量,0表示不会攻击
    cxFloat attackPower;    //攻击力
    //水果创建者
    CX_METHOD_ALLOC(cxAny, FruitMaker,cxAny this);
CX_OBJECT_END(fcAttacker)
//暂停一段时间
void fcAttackerPauseTime(cxAny this,cxFloat time);
//暂停攻击
void fcAttackerPause(cxAny this);
//继续攻击
void fcAttackerResume(cxAny this);
//创建攻击者
fcAttacker fcAttackerCreate(cxAny map,cxVec2i idx,fcAttackerType type);
//开始搜索攻击
void fcAttackerLoop(cxAny this);
CX_C_END

#endif
