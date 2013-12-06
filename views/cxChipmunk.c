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
    cxViewSetPosImp(body->data, cxVec2fv(body->p.x, body->p.y));
    cxViewSetRadiansImp(body->data, body->a);
}

static void cxChipmunkSetPos(cxAny pview,cxVec2f pos)
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

static void cxChipmunkSetRadians(cxAny pview,cxFloat angle)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpBody *body = cpShapeGetBody(shape);
    CX_RETURN(cpBodyIsStatic(body));
    cpBodySetAngle(body, angle);
}

void cxViewSetElasticity(cxAny pview,cxFloat e)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpShapeSetElasticity(shape, e);
}

void cxViewSetFriction(cxAny pview,cxFloat u)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpShapeSetFriction(shape, u);
}

void cxViewSetMass(cxAny pview,cxFloat m)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpBody *body = cpShapeGetBody(shape);
    cpBodySetMass(body, m);
}

void cxViewSetMoment(cxAny pview,cxFloat i)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpBody *body = cpShapeGetBody(shape);
    cpBodySetMoment(body, i);
}

void cxViewResetForces(cxAny pview)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpBody *body = cpShapeGetBody(shape);
    cpBodyResetForces(body);
}

void cxViewApplyForce(cxAny pview, cxVec2f force, cxVec2f r)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpBody *body = cpShapeGetBody(shape);
    cpBodyApplyForce(body, cpv(force.x, force.y), cpv(r.x, r.y));
}

void cxViewApplyImpulse(cxAny pview, const cxVec2f j, const cxVec2f r)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    cpBody *body = cpShapeGetBody(shape);
    cpBodyApplyImpulse(body, cpv(j.x, j.y), cpv(r.x, r.y));
}

void cxViewSetCollisionType(cxAny pview,cxUInt type)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    cpShapeSetCollisionType(shape, type);
}

void cxViewSetGroup(cxAny pview,cxUInt group)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    cpShapeSetGroup(shape, group);
}

void cxViewSetLayers(cxAny pview,cxUInt layers)
{
    CX_ASSERT(pview != NULL, "view null");
    cxView this = pview;
    CX_RETURN(this->chipmunk.shape == NULL);
    cpShape *shape = this->chipmunk.shape;
    cpShapeSetLayers(shape, layers);
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
    cpShapeSetLayers(shape, view->chipmunk.layer);
    cpShapeSetGroup(shape, view->chipmunk.group);
    cpShapeSetCollisionType(shape, view->chipmunk.ctype);
    cpShapeSetElasticity(shape, view->chipmunk.e);
    cpShapeSetFriction(shape, view->chipmunk.u);
    cpShapeSetUserData(shape, nview);
    view->chipmunk.shape = shape;
    cpSpaceAddShape(this->space, shape);
    //
    CX_METHOD_SET(view->SetPosition, cxChipmunkSetPos);
    CX_METHOD_SET(view->SetRadians, cxChipmunkSetRadians);
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





