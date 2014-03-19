//
//  yySprite.c
//  yaoyao
//
//  Created by xuhua on 3/17/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "yyMap.h"
#include "yySprite.h"

CX_OBJECT_INIT(yySprite, cxSprite)
{
    this->angle = yyAngleRight;
    this->speed = 1;
}
CX_OBJECT_FREE(yySprite, cxSprite)
{
    
}
CX_OBJECT_TERM(yySprite, cxSprite)

yySprite yySpriteCreate(cxAny map,cxUInt type,cxVec2i idx)
{
    yyMap m = map;
    yySprite this = CX_CREATE(yySprite);
    this->type = type;
    this->map = map;
    cxSpriteSetTextureURL(this, "map.xml?item.png", false);
    cxViewSetSize(this, m->itemSize);
    cxVec2f pos = yyMapGetPos(map, idx);
    cxViewSetPos(this, pos);
    return this;
}

void yySpriteSetAngle(yySprite this,yyAngle angle)
{
    if(angle == yyAngleLeft){
        cxViewSetAngle(this, kmDegreesToRadians(180));
    }else if(angle == yyAngleRight){
        cxViewSetAngle(this, kmDegreesToRadians(0));
    }else if(angle == yyAngleUp){
        cxViewSetAngle(this, kmDegreesToRadians(90));
    }else if(angle == yyAngleDown){
        cxViewSetAngle(this, kmDegreesToRadians(270));
    }
    this->angle = angle;
}

