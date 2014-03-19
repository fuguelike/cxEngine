//
//  yySprite.h
//  cxEngine
//
//  Created by xuhua on 3/17/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef yaoyao_yySprite_h
#define yaoyao_yySprite_h

#include <views/cxSprite.h>

CX_C_BEGIN

typedef enum {
    yyAngleNone  = 0,
    yyAngleLeft  = 1 << 0,
    yyAngleRight = 1 << 1,
    yyAngleUp    = 1 << 2,
    yyAngleDown  = 1 << 3,
}yyAngle;

CX_OBJECT_DEF(yySprite, cxSprite)
    cxAny map;
    cxFloat speed;
    yyAngle angle;
    cxUInt type;
CX_OBJECT_END(yySprite)

yySprite yySpriteCreate(cxAny map,cxUInt type,cxVec2i idx);

void yySpriteSetAngle(yySprite this,yyAngle angle);

CX_C_END

#endif
