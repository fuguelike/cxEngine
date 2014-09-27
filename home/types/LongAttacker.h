//
//  LongAttacker.h
//  Home
//  远程地面攻击者，例如弓箭手
//  Created by xuhua on 9/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_LongAttacker_h
#define Home_LongAttacker_h

#include "Attack.h"

CX_C_BEGIN

CX_OBJECT_DEF(LongAttacker, Attack)
CX_OBJECT_END(LongAttacker, Attack)

void LongAttackerInit(cxAny pview, cxAny pmap,cxVec2i pos);

LongAttacker LongAttackerCreate(cxAny pmap,cxVec2i pos);

CX_C_END

#endif
