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
    this->oldRadians = this->super.view->radians;
    this->delta = this->newRadians;
    cxViewSetRaxis(this->super.view, this->raxis);
}

static void cxRotateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxRotate this = pav;
    cxFloat radians = this->oldRadians + this->delta * time;
    cxViewSetRadians(this->super.view, radians);
}

static cxAny cxRotateClone(cxAny pav)
{
    cxRotate this = pav;
    cxRotate rv = CX_CREATE(cxRotate);
    rv->raxis = this->raxis;
    rv->newRadians = this->newRadians;
    return rv;
}

static void cxRotateXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXMLReadAttr(xmlAction, mAction, reader);
    cxRotate this = mAction;
    cxChar *sx = cxXMLAttr("cxRotate.x");
    cxChar *sy = cxXMLAttr("cxRotate.y");
    cxChar *sz = cxXMLAttr("cxRotate.z");
    if(sx != NULL){
        this->raxis = cxVec3fv(1.0f, 0.0f, 0.0f);
        this->newRadians = kmDegreesToRadians(atof(sx));
    }else if(sy != NULL){
        this->raxis = cxVec3fv(0.0f, 1.0f, 0.0f);
        this->newRadians = kmDegreesToRadians(atof(sy));
    }else if(sz != NULL){
        this->raxis = cxVec3fv(0.0f, 0.0f, 1.0f);
        this->newRadians = kmDegreesToRadians(atof(sz));
    }
    xmlFree(sx);
    xmlFree(sy);
    xmlFree(sz);
}

CX_OBJECT_INIT(cxRotate, cxAction)
{
    this->raxis = cxVec3fv(0.0f, 0.0f, 1.0f);
    cxObjectSetXMLReadFunc(this, cxRotateXMLReadAttr);
    CX_METHOD_SET(this->super.Init, cxRotateInit);
    CX_METHOD_SET(this->super.Step, cxRotateStep);
    CX_METHOD_SET(this->super.Clone, cxRotateClone);
}
CX_OBJECT_FREE(cxRotate, cxAction)
{

}
CX_OBJECT_TERM(cxRotate, cxAction)

cxRotate cxRotateCreate(cxFloat duration,cxVec3f raxis,cxFloat newRadians)
{
    cxRotate this = CX_CREATE(cxRotate);
    this->super.duration = duration;
    this->raxis = raxis;
    this->newRadians = newRadians;
    return this;
}


