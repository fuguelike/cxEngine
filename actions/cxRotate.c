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
    CX_ASSERT_TYPE(this->cxAction.view, cxView);
    this->oldAngle = this->cxAction.view->angle;
    this->delta = this->newAngle - this->oldAngle;
    cxViewSetRaxis(this->cxAction.view, this->raxis);
}

static void cxRotateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxRotate);
    cxFloat angle = this->oldAngle + this->delta * time;
    cxViewSetAngle(this->cxAction.view, angle);
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

CX_OBJECT_TYPE(cxRotate, cxAction)
{
    CX_PROPERTY_SETTER(cxRotate, raxis);
    CX_PROPERTY_SETTER(cxRotate, angle);
    CX_PROPERTY_SETTER(cxRotate, degrees);
}
CX_OBJECT_INIT(cxRotate, cxAction)
{
    this->raxis = cxVec3fZ;
    CX_METHOD_SET(this->cxAction.Init, cxRotateInit);
    CX_METHOD_SET(this->cxAction.Step, cxRotateStep);
}
CX_OBJECT_FREE(cxRotate, cxAction)
{

}
CX_OBJECT_TERM(cxRotate, cxAction)

cxRotate cxRotateCreate(cxFloat time,cxVec3f raxis,cxFloat newAngle)
{
    cxRotate this = CX_CREATE(cxRotate);
    this->cxAction.time = time;
    this->raxis = raxis;
    this->newAngle = newAngle;
    return this;
}


