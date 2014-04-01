//
//  fcLocation.c
//  fruit
//
//  Created by xuhua on 4/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcMap.h"
#include "fcLocation.h"

static cxBool fcLocationTouch(cxAny pview,cxTouch *touch)
{
    fcLocation this = pview;
    fcMap map = fcSpriteMapView(pview);
    cxVec2f pos = cxVec2fv(0, 0);
    if(fcSpriteTouch(pview, touch)){
        return true;
    }
    if(map->mode != fcMapModeFire){
        return false;
    }
    if(!cxViewHitTest(pview, touch->current, &pos)){
        return false;
    }
    if(touch->type == cxTouchTypeUp){
        //点击到起点或者终点
        CX_METHOD_RUN(map->Location, map, this);
        CX_LOGGER("touch locaton %d",this->type);
        return true;
    }
    return false;
}

CX_OBJECT_INIT(fcLocation, fcSprite)
{
    CX_METHOD_OVERRIDE(this->super.super.super.Touch, fcLocationTouch);
    this->super.type = fcSpriteTypeDecoration;
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