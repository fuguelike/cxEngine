//
//  Defence.h
//  Home
//  被动防御单位炮塔，防御单位
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Defence_h
#define Home_Defence_h

#include <Node.h>

CX_C_BEGIN

CX_DEF(Defence, Node)
    
CX_END(Defence, Node)

Defence DefenceCreate(cxAny pmap,cxVec2i idx);

FindRuleResult DefenceFindRule(cxAny pview,const NodeCombined *type);

PathRuleResult DefencePathRule(cxAny pview,const FindRuleResult *fret);

ActionResult DefenceAttackAction(cxAny pattacker,cxAny ptarget);

CX_C_END

#endif
