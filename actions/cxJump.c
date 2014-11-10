//
//  cxJump.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxJump.h"

CX_METHOD_DEF(cxJump,Step,void,cxFloat dt, cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxFloat frac = fmodf( time * this->jumps, 1.0f );
    cxFloat y = this->height * 4 * frac * (1 - frac) + this->position.y * time;
    cxFloat x = this->position.x * time;
    cxVec2f currPos = cxViewGetPosition(view);
    cxVec2f diff = cxVec2fSub(currPos, this->prevPos);
    this->startPos = cxVec2fAdd(this->startPos, diff);
    cxVec2f nPos = cxVec2fAdd(this->startPos, cxVec2fv(x, y));
    cxViewSetPosition(view, nPos);
    this->prevPos = nPos;
}
CX_METHOD_DEF(cxJump,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->prevPos = this->startPos = cxViewGetPosition(view);
    CX_SUPER(cxAction, this, Init, CX_M(void));
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
CX_TYPE(cxJump, cxAction)
{
    CX_SETTER(cxJump, position);
    CX_SETTER(cxJump, height);
    CX_SETTER(cxJump, jumps);
    
    CX_METHOD(cxJump, Init);
    CX_METHOD(cxJump, Step);
}
CX_INIT(cxJump, cxAction)
{

}
CX_FREE(cxJump, cxAction)
{
    //
}
CX_TERM(cxJump, cxAction)

cxJump cxJumpCreate(cxFloat time,cxVec2f position,cxFloat height,cxInt jumps)
{
    cxJump this = CX_CREATE(cxJump);
    this->cxAction.Time = time;
    this->position = position;
    this->height = height;
    this->jumps = jumps;
    return this;
}

