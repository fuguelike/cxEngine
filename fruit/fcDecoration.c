//
//  fcDecoration.c
//  fruit
//
//  Created by xuhua on 4/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcDecoration.h"

CX_OBJECT_INIT(fcDecoration, fcSprite)
{
    this->super.type = fcSpriteTypeDecoration;
}
CX_OBJECT_FREE(fcDecoration, fcSprite)
{

}
CX_OBJECT_TERM(fcDecoration, fcSprite)

void fcDecorationInit(cxAny pview,cxAny map,fcDecorationType type)
{
    fcDecoration this = pview;
    this->type = type;
    fcSpriteInit(this, map);
}