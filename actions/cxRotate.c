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
    cxRotate this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->oldAngle = this->super.view->angle;
    this->delta = this->newAngle - this->oldAngle;
    cxViewSetRaxis(this->super.view, this->raxis);
}

static void cxRotateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxRotate this = pav;
    cxFloat angle = this->oldAngle + this->delta * time;
    cxViewSetAngle(this->super.view, angle);
}

static void cxRotateReadAttr(cxReaderAttrInfo *info)
{
    cxActionReadAttr(info);
    cxRotate this = info->object;
    cxConstChars sx = cxXMLAttr(info->reader,"cxRotate.x");
    cxConstChars sy = cxXMLAttr(info->reader,"cxRotate.y");
    cxConstChars sz = cxXMLAttr(info->reader,"cxRotate.z");
    if(sx != NULL){
        this->raxis = cxVec3fv(1.0f, 0.0f, 0.0f);
        this->newAngle = kmDegreesToRadians(atof(sx));
    }else if(sy != NULL){
        this->raxis = cxVec3fv(0.0f, 1.0f, 0.0f);
        this->newAngle = kmDegreesToRadians(atof(sy));
    }else if(sz != NULL){
        this->raxis = cxVec3fv(0.0f, 0.0f, 1.0f);
        this->newAngle = kmDegreesToRadians(atof(sz));
    }
}

CX_OBJECT_INIT(cxRotate, cxAction)
{
    this->raxis = cxVec3fv(0.0f, 0.0f, 1.0f);
    cxObjectSetReadAttrFunc(this, cxRotateReadAttr);
    CX_METHOD_OVERRIDE(this->super.Init, cxRotateInit);
    CX_METHOD_OVERRIDE(this->super.Step, cxRotateStep);
}
CX_OBJECT_FREE(cxRotate, cxAction)
{

}
CX_OBJECT_TERM(cxRotate, cxAction)

cxRotate cxRotateCreate(cxFloat duration,cxVec3f raxis,cxFloat newAngle)
{
    cxRotate this = CX_CREATE(cxRotate);
    this->super.duration = duration;
    this->raxis = raxis;
    this->newAngle = newAngle;
    return this;
}


