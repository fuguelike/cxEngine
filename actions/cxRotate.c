//
//  cxRotate.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxRotate.h"

CX_METHOD_DEF(cxRotate,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->oldAngle = cxViewGetAngle(view);
    this->delta = this->newAngle - this->oldAngle;
    cxViewSetRaxis(view, this->raxis);
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxRotate,Step,void,cxFloat dt,cxFloat time)
{
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
    
    CX_METHOD(cxRotate, Init);
    CX_METHOD(cxRotate, Step);
}
CX_INIT(cxRotate, cxAction)
{
    this->raxis = cxVec3fZ;
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


