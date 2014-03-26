//
//  fcSprite.h
//  cxEngine
//
//  Created by xuhua on 3/21/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcSprite_h
#define fruit_fcSprite_h

#include <actions/cxTimer.h>
#include <views/cxSprite.h>
#include "fcDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcSprite, cxSprite)
    cxAny map;
    cxVec2i idx;
    fcSpriteType type;
    //攻击目标一个或多个,当sprite死去的时候，记得从所有的murderers的targets中删除
    cxHash targets;
    //盯住我的凶手,当攻击目标时记得把目标的murderers加上自己
    cxHash murderers;
CX_OBJECT_END(fcSprite)

//s1 指向 s2
void fcSpriteLookAt(cxAny s1,cxAny s2);

//获取两个sprite间的距离
cxFloat fcSpriteDistance(cxAny s1,cxAny s2);

//解除

//解除所有关系
void fcSpriteUnset(cxAny this);

//设置目标
void fcSpriteTarget(cxAny this,cxAny target);

void fcSpriteInit(cxAny this,cxAny map,cxVec2i idx);

//创建定时器
cxTimer fcSpriteTimer(cxAny this,cxFloat time);

CX_C_END

#endif
