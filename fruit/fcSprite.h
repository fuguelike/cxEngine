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
    //当目标结束时，关系已经被解除
    CX_EVENT_ALLOC(onOver);
CX_OBJECT_END(fcSprite)

void fcSpriteOver(cxAny this);

void fcSpriteTarget(cxAny this,cxAny target);

void fcSpriteInit(cxAny this,cxAny map,cxVec2i idx);

//创建定时器
cxTimer fcSpriteTimer(cxAny this,cxFloat time);

CX_C_END

#endif
