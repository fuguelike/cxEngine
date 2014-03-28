//
//  fcSpriteMove.c
//  fruit
//
//  Created by xuhua on 3/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcMap.h"
#include "fcSprite.h"
#include "fcSpriteMove.h"

static cxVec2f fcSpriteMoveTargetPos(cxAny pav)
{
    fcSpriteMove move = pav;
    fcSprite s = cxActionView(move);
    cxVec2i idx = s->path.points[move->pointIndex];
    return fcMapToPos(s->map, idx);
}

static cxBool fcSpriteMoveExit(cxAny pav)
{
    fcSpriteMove this = pav;
    fcSprite sprite = cxActionView(pav);
    cxVec2f tp = fcSpriteMoveTargetPos(pav);
    cxVec2f cp = cxViewPosition(sprite);
    cxFloat distance = kmVec2DistanceBetween(&tp, &cp);
    if(distance > this->super.minDistance){
        return false;
    }
    if(this->pointIndex >= (sprite->path.number - 1)){
        return true;
    }
    this->pointIndex ++;
    cxVec2f np = fcSpriteMoveTargetPos(pav);
    cxFollowSetVec2f(pav, np);
    return false;
}

static void fcSpriteMoveInit(cxAny pav)
{
    fcSpriteMove this = pav;
    this->super.minDistance = 3;
}

CX_OBJECT_INIT(fcSpriteMove, cxFollow)
{
    this->pointIndex = 1;
    CX_METHOD_OVERRIDE(this->super.super.Init, fcSpriteMoveInit);
    CX_METHOD_OVERRIDE(this->super.Exit, fcSpriteMoveExit);
}
CX_OBJECT_FREE(fcSpriteMove, cxFollow)
{
    
}
CX_OBJECT_TERM(fcSpriteMove, cxFollow)

void fcSpriteMoveLoop(cxAny sprite,cxVec2i d)
{
    fcSprite s = sprite;
    if(fcSpriteFindPath(s, d) && s->path.number > 1){
        fcSpriteMove move = CX_CREATE(fcSpriteMove);
        cxActionSetId(move, FC_MOVE_ACTION_ID);
        cxFloat speed = fcMapScaleValue(s->map, s->speed);
        cxFollowSetInitSpeed(move, speed);
        cxViewAppendAction(s, move);
        cxVec2f pt = fcSpriteMoveTargetPos(move);
        cxFollowSetVec2f(move, pt);
    }
}



