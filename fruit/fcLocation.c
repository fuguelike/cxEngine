//
//  fcLocation.c
//  fruit
//
//  Created by xuhua on 4/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcLocation.h"

CX_OBJECT_INIT(fcLocation, fcSprite)
{
    
}
CX_OBJECT_FREE(fcLocation, fcSprite)
{

}
CX_OBJECT_TERM(fcLocation, fcSprite)

void fcLocationInit(cxAny this,cxAny map,fcLocationType type)
{
    fcLocation l = this;
    l->type = type;
    fcSpriteInit(this, map);
}