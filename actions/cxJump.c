//
//  cxJump.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxJump.h"

static void cxJumpInit(cxAny pav)
{
    cxJump this = pav;
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    this->prevPos = this->startPos = this->cxAction.view->position;
}

static void cxJumpStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxJump this = pav;
    cxFloat frac = fmodf( time * this->jumps, 1.0f );
    cxFloat y = this->height * 4 * frac * (1 - frac) + this->position.y * time;
    cxFloat x = this->position.x * time;
    cxVec2f diff;
    cxVec2f currPos = this->cxAction.view->position;
    kmVec2Subtract(&diff, &currPos, &this->prevPos);
    kmVec2Add(&this->startPos, &diff, &this->startPos);
    cxVec2f nPos;
    kmVec2Add(&nPos, &this->startPos, &cxVec2fv(x, y));
    this->prevPos = nPos;
    cxViewSetPos(this->cxAction.view, nPos);
}

CX_SETTER_DEF(cxJump, position)
{
    this->position = cxJsonToVec2f(value, this->position);
}
CX_SETTER_DEF(cxJump, height)
{
    this->height = cxJsonToDouble(value, this->height);
}
CX_SETTER_DEF(cxJump, jumps)
{
    this->jumps = cxJsonToInt(value, this->jumps);
}

CX_OBJECT_TYPE(cxJump, cxAction)
{
    CX_PROPERTY_SETTER(cxJump, position);
    CX_PROPERTY_SETTER(cxJump, height);
    CX_PROPERTY_SETTER(cxJump, jumps);
}
CX_OBJECT_INIT(cxJump, cxAction)
{
    CX_METHOD_SET(this->cxAction.Init, cxJumpInit);
    CX_METHOD_SET(this->cxAction.Step, cxJumpStep);
}
CX_OBJECT_FREE(cxJump, cxAction)
{
    //
}
CX_OBJECT_TERM(cxJump, cxAction)

cxJump cxJumpCreate(cxFloat duration,cxVec2f position,cxFloat height,cxUInt jumps)
{
    cxJump this = CX_CREATE(cxJump);
    this->cxAction.duration = duration;
    this->position = position;
    this->height = height;
    this->jumps = jumps;
    return this;
}

