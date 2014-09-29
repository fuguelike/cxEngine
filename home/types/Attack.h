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

CX_OBJECT_END(Attack, Node)

void AttackInit(cxAny pview, cxAny pmap,cxVec2i pos);

Attack AttackCreate(cxAny pmap, cxVec2i pos);

PathRuleResult AttackPathRule(cxAny pview,FindRuleResult *fret);

FindRuleResult AttackFindRule(cxAny pview,const NodeCombined *type);

AttackActionResult AttackAttackAction(cxAny pattacker,cxAny ptarget);

CX_C_END

#endif
