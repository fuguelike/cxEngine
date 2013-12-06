//
//  cxChipmunk.c
//  cxEngine
//
//  Created by xuhua on 11/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxViewXML.h"
#include "cxChipmunk.h"

static void cxChipmunkUpdate(cxEvent *event)
{
    cxEngine engine = cxEngineInstance();
    cxChipmunk this = event->sender;
    cpSpaceStep(this->space, engine->frameInterval);
}

void cxChipmunkSetGravity(cxAny pview,cxVec2f gravity)
{
    cxChipmunk this = pview;
    cpSpaceSetGravity(this->space, cpv(gravity.x, gravity.y));
}

cxBool cxChipmunkXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxChipmunk this = mView;
    cpVect gravity = this->space->gravity;
    cxXMLReadFloatsAttr(reader, "cxChipmunk.gravity", &gravity.x);
    cpSpaceSetGravity(this->space, gravity);
    return true;
}

static void cxChipmunkUpdatePosition(cpBody *body, cpFloat dt)
{
    cpBodyUpdatePosition(body, dt);
    cxViewSetPosition(body->data, cxVec2fv(body->p.x, body->p.y));
    cxViewSetRadians(body->data, body->a);
}

void cxChipmunkAppendAfter(cxAny pview,cxAny nview)
{
    cxChipmunk this = pview;
    cxView view  = nview;
    
    cxSize2f size = cxViewSize(nview);
    cxVec2f pos = cxViewPosition(nview);
    cxFloat radians = cxViewRadians(nview);
    
    cpBody *body = NULL;
    if(view->chipmunk.isStatic){
        body = cpBodyNewStatic();
    }else{
        body = cpBodyNew(view->chipmunk.m, cpMomentForBox(view->chipmunk.m, size.w, size.h));
        cpSpaceAddBody(this->space, body);
    }
    
    cpBodySetAngle(body, radians);
    cpBodySetPos(body, cpv(pos.x, pos.y));
    cpBodySetUserData(body, nview);
    body->position_func = cxChipmunkUpdatePosition;
    
    cpShape *shape = cpBoxShapeNew(body, size.w, size.h);
    cpShapeSetElasticity(shape, view->chipmunk.e);
    cpShapeSetFriction(shape, view->chipmunk.u);
    cpShapeSetUserData(shape, nview);
    view->chipmunk.shape = shape;
    cpSpaceAddShape(this->space, shape);
}

void cxChipmunkSetPosition(cxAny pview,cxVec2f pos)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpBody *body = cpShapeGetBody(shape);
    CX_RETURN(cpBodyIsStatic(body));
    cpBodySetPos(body, cpv(pos.x, pos.y));
}

void cxChipmunkRemoveBefore(cxAny pview,cxAny nview)
{
    cxChipmunk this = pview;
    cxView view  = nview;
    cpShape *shape = view->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpBody *body = cpShapeGetBody(shape);
    if(!cpBodyIsStatic(body)){
        cpSpaceRemoveBody(this->space, body);
    }
    cpBodyFree(body);
    cpSpaceRemoveShape(this->space, shape);
    cpShapeFree(shape);
    view->chipmunk.shape = NULL;
}

CX_OBJECT_INIT(cxChipmunk, cxView)
{
    CX_METHOD_SET(this->super.AppendAfter, cxChipmunkAppendAfter);
    CX_METHOD_SET(this->super.RemoveBefore, cxChipmunkRemoveBefore);
    cxObjectSetXMLReadFunc(this, cxChipmunkXMLReadAttr);
    this->space = cpSpaceNew();
    CX_EVENT_QUICK(this->super.onUpdate, cxChipmunkUpdate);
    cpSpaceSetGravity(this->space, cpv(0, -1000));
}
CX_OBJECT_FREE(cxChipmunk, cxView)
{
    cpSpaceFree(this->space);
}
CX_OBJECT_TERM(cxChipmunk, cxView)





