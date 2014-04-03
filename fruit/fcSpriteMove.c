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
    //未到达目的点
    if(distance > this->super.min){
        return false;
    }
    //已经到最后一点
    if(this->pointIndex >= (sprite->path.number - 1)){
        //更新位置索引
        sprite->idx = fcMapToIdx(sprite->map, cp);
        return true;
    }
    //设置到下一点目标
    this->pointIndex ++;
    cxVec2f np = fcSpriteMoveTargetPos(pav);
    cxFollowSetVec2f(pav, np);
    return false;
}

static cxFloat fcSpriteMoveSpeed(cxAny pav,cxFloat time)
{
    fcSpriteMove this = pav;
    fcSprite s = cxActionView(pav);
    cxFloat us = fcMapScaleValue(s->map, this->speed);
    return (this->super.init + us);
}

static void fcSpriteMoveInit(cxAny pav)
{
    fcSpriteMove this = pav;
    this->super.min = 3;
}

CX_OBJECT_INIT(fcSpriteMove, cxFollow)
{
    this->speed = 0;
    this->pointIndex = 1;
    CX_METHOD_OVERRIDE(this->super.Speed, fcSpriteMoveSpeed);
    CX_METHOD_OVERRIDE(this->super.super.Init, fcSpriteMoveInit);
    CX_METHOD_OVERRIDE(this->super.Exit, fcSpriteMoveExit);
}
CX_OBJECT_FREE(fcSpriteMove, cxFollow)
{
    
}
CX_OBJECT_TERM(fcSpriteMove, cxFollow)

cxAny fcSpriteMoveAction(cxAny pview)
{
    return cxViewGetAction(pview, FC_MOVE_ACTION_ID);
}

void fcSpriteStop(cxAny sprite)
{
    cxAny pav = fcSpriteMoveAction(sprite);
    if(pav != NULL){
        cxActionStop(pav);
    }
}

static void fcSpriteMoveStop(cxEvent *e)
{
    fcSprite s = cxActionView(e->sender);
    fcMap map = s->map;
    cxVec2i eIdx = fcMapEndIndex(map);
    if(!cxVec2iEqu(s->idx, eIdx)){
        return;
    }
    CX_METHOD_RUN(map->Arrive, map, s);
}

void fcSpriteMoveLoop(cxAny sprite)
{
    fcSprite s = sprite;
    if(!fcSpriteFindEndLocationPath(s)){
        return;
    }
    if(s->path.number < 2){
        return;
    }
    fcSpriteMove move = CX_CREATE(fcSpriteMove);
    cxActionSetId(move, FC_MOVE_ACTION_ID);
    cxFloat speed = fcMapScaleValue(s->map, s->speed);
    cxFollowSetInit(move, speed);
    cxViewAppendAction(s, move);
    cxVec2f pt = fcSpriteMoveTargetPos(move);
    cxFollowSetVec2f(move, pt);
    CX_EVENT_QUICK(move->super.super.onStop, fcSpriteMoveStop);
}





