//
//  fcFruit.h
//  cxEngine
//
//  Created by xuhua on 3/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcFruit_h
#define fruit_fcFruit_h

#include <views/cxSprite.h>
#include "fcDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcFruit, cxSprite)
    fcFruitType type;
    cxAny map;
    cxFloat speed;
    cxFloat attackPower;
    //当碰到时
    CX_METHOD_ALLOC(cxInt, Fire,cxAny fruit,cxAny sprite,cxAny target);
CX_OBJECT_END(fcFruit)

void fcFruitRemoved(cxAny this);

void fcFruitInit(cxAny this,cxAny map,cxConstChars url,cxFloat speed);

//fruit 从sprite 向 target 开火
cxInt fcFruitFire(cxAny this,cxAny sprite,cxAny target);

CX_C_END

#endif
