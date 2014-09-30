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
    CX_ASSERT_THIS(pav, cxJump);
    CX_ASSERT_VALUE(cxActionView(this), cxView, view);
    this->prevPos = this->startPos = cxViewGetPosition(view);
}

static void cxJumpStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxJump);
    CX_ASSERT_VALUE(cxActionView(this), cxView, view);
    cxFloat frac = fmodf( time * this->jumps, 1.0f );
    cxFloat y = this->height * 4 * frac * (1 - frac) + this->position.y * time;
    cxFloat x = this->position.x * time;
    cxVec2f diff;
    cxVec2f currPos = cxViewGetPosition(view);
    kmVec2Subtract(&diff, &currPos, &this->prevPos);
    kmVec2Add(&this->startPos, &diff, &this->startPos);
    cxVec2f nPos;
    kmVec2Add(&nPos, &this->startPos, &cxVec2fv(x, y));
    cxViewSetPosition(view, nPos);
    this->prevPos = nPos;
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
    SET(cxAction, this, Init, cxJumpInit);
    SET(cxAction, this, Step, cxJumpStep);
}
CX_OBJECT_FREE(cxJump, cxAction)
{
    //
}
CX_OBJECT_TERM(cxJump, cxAction)

cxJump cxJumpCreate(cxFloat time,cxVec2f position,cxFloat height,cxInt jumps)
{
    cxJump this = CX_CREATE(cxJump);
    this->cxAction.time = time;
    this->position = position;
    this->height = height;
    this->jumps = jumps;
    return this;
}

