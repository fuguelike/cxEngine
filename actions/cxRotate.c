//
//  cxRotate.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxRotate.h"

static void cxRotateInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxRotate);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->oldAngle = cxViewGetAngle(view);
    this->delta = this->newAngle - this->oldAngle;
    cxViewSetRaxis(view, this->raxis);
}

static void cxRotateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxRotate);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxFloat angle = this->oldAngle + this->delta * time;
    cxViewSetAngle(view, angle);
}

CX_SETTER_DEF(cxRotate, raxis)
{
    this->raxis = cxJsonToVec3f(value, this->raxis);
}
CX_SETTER_DEF(cxRotate, angle)
{
    this->newAngle = cxJsonToDouble(value, this->newAngle);
}
CX_SETTER_DEF(cxRotate, degrees)
{
    cxFloat oldDegrees = kmRadiansToDegrees(this->newAngle);
    this->newAngle = kmDegreesToRadians(cxJsonToDouble(value, oldDegrees));
}

CX_TYPE(cxRotate, cxAction)
{
    CX_SETTER(cxRotate, raxis);
    CX_SETTER(cxRotate, angle);
    CX_SETTER(cxRotate, degrees);
}
CX_INIT(cxRotate, cxAction)
{
    this->raxis = cxVec3fZ;
    CX_SET(cxAction, this, Init, cxRotateInit);
    CX_SET(cxAction, this, Step, cxRotateStep);
}
CX_FREE(cxRotate, cxAction)
{

}
CX_TERM(cxRotate, cxAction)

cxRotate cxRotateCreate(cxFloat time,cxVec3f raxis,cxFloat newAngle)
{
    cxRotate this = CX_CREATE(cxRotate);
    this->cxAction.Time = time;
    this->raxis = raxis;
    this->newAngle = newAngle;
    return this;
}


