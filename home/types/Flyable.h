//
//  Flyable.h
//  cxCore
//  飞行单位，具有攻击性，无需寻路
//  Created by xuhua on 9/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Flyable_h
#define Home_Flyable_h

#include "Attack.h"

CX_C_BEGIN

CX_OBJECT_DEF(Flyable, Attack)
CX_OBJECT_END(Flyable, Attack)

ActionResult FlyableAttackAction(cxAny pattacker,cxAny ptarget);

PathRuleResult FlyablePathRule(cxAny pview,const FindRuleResult *fret);

FindRuleResult FlyableFindRule(cxAny pview,const NodeCombined *type);

void FlyableInit(cxAny pview, cxAny pmap,cxVec2i idx);

Flyable FlyableCreate(cxAny pmap,cxVec2i idx);

CX_C_END

#endif
