//
//  cxChipmunk.c
//  cxEngine
//
//  Created by xuhua on 11/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxViewRoot.h>
#include "cxChipmunk.h"

CX_LUA_METHOD_BEG(cxChipmunk)

CX_LUA_METHOD_END(cxChipmunk)

void __cxChipmunkTypeInit()
{
    CX_LUA_LOAD_TYPE(cxChipmunk);
}

static void cxChipmunkUpdate(cxEvent *event)
{
    cxEngine engine = cxEngineInstance();
    cxChipmunk this = event->sender;
    cpSpaceStep(this->space, engine->frameDelta);
}

void cxChipmunkSetGravity(cxAny pview,cxVec2f gravity)
{
    cxChipmunk this = pview;
    cpSpaceSetGravity(this->space, gravity);
}

void cxChipmunkReadAttr(cxReaderAttrInfo *info)
{
    cxViewReadAttr(info);
    cxChipmunk this = info->object;
    //set space
    cpVect gravity = cpSpaceGetGravity(this->space);
    cxVec2f v = cxXMLReadVec2fAttr(info, "cxChipmunk.gravity", gravity);
    cpSpaceSetGravity(this->space, v);
}

static void cxChipmunkBodyUpdatePosition(cpBody *body, cpFloat dt)
{
    cpBodyUpdatePosition(body, dt);
    cxViewSetPos(body->data, body->p);
    cxViewSetAngle(body->data, body->a);
}

static void cxChipmunkBodyUpdateVelocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
    cpBodyUpdateVelocity(body, gravity, damping, dt);
}

void cxChipmunkSetPos(cxAny pview,cxVec2f pos)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    CX_RETURN(cpBodyIsStatic(body));
    cpBodySetPos(body, pos);
}

void cxChipmunkSetAngle(cxAny pview,cxFloat angle)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    CX_RETURN(cpBodyIsStatic(body));
    cpBodySetAngle(body, angle);
}

void cxChipmunkSetElasticity(cxAny pview,cxFloat e)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpShapeSetElasticity(shape, e);
}

void cxChipmunkSetFriction(cxAny pview,cxFloat u)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpShapeSetFriction(shape, u);
}

void cxChipmunkSetMass(cxAny pview,cxFloat m)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    cpBodySetMass(body, m);
}

void cxChipmunkResetForces(cxAny pview)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    cpBodyResetForces(body);
}

void cxChipmunkApplyForce(cxAny pview, cxVec2f force, cxVec2f r)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    cpBodyApplyForce(body, force, r);
}

void cxChipmunkApplyImpulse(cxAny pview, const cxVec2f j, const cxVec2f r)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    cpBodyApplyImpulse(body, j, r);
}

void cxChipmunkSetCollisionType(cxAny pview,cxUInt type)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpShapeSetCollisionType(shape, type);
}

void cxChipmunkSetGroup(cxAny pview,cxUInt group)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpShapeSetGroup(shape, group);
}

void cxChipmunkSetLayers(cxAny pview,cxUInt layers)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpShapeSetLayers(shape, layers);
}

static void cpBodySetPositionFunc(cpBody *body,cpBodyPositionFunc func)
{
    CX_ASSERT(body != NULL && func != NULL, "args error");
    body->position_func = func;
}

static void cpBodySetVelocityFunc(cpBody *body,cpBodyVelocityFunc func)
{
    CX_ASSERT(body != NULL && func != NULL, "args error");
    body->velocity_func = func;
}

cxChipmunkAttr *cxChipmunkAttrCreate(cxBool isStatic)
{
    cxChipmunkAttr *attr = cxMemoryCreate(sizeof(cxChipmunkAttr));
    attr->cp = cxVec2fv(0, 0);
    attr->ctype = 0;
    attr->e = 0.0f;
    attr->group = CP_NO_GROUP;
    attr->isStatic = isStatic;
    attr->layer = CP_ALL_LAYERS;
    attr->m = 1.0f;
    attr->shape = cxChipmunkShapeBox;
    attr->u = 0.0f;
    return attr;
}

cxAny cxChipmunkAppend(cxAny pview,cxAny nview,cxChipmunkAttr *attr)
{
    cxChipmunk this = pview;
    CX_ASSERT(cxViewArgs(nview) == NULL,"view's args not null");
    cxSize2f size = cxViewSize(nview);
    CX_RETURN(cxSize2Zero(size), NULL);
    cxVec2f pos = cxViewPosition(nview);
    cxFloat angle = cxViewAngle(nview);
    cpBody *body = NULL;
    cxFloat i = 0;
    cxFloat r = CX_MAX(size.w, size.h) / 2.0f;
    if(attr->shape == cxChipmunkShapeBox){
        i = cpMomentForBox(attr->m, size.w, size.h);
    }else if(attr->shape == cxChipmunkShapeCircle){
        i = cpMomentForCircle(attr->m, 0, r, attr->cp);
    }else{
        CX_ERROR("shape error");
        return NULL;
    }
    if(attr->isStatic){
        body = cpBodyNewStatic();
    }else{
        body = cpBodyNew(attr->m, i);
        cpSpaceAddBody(this->space, body);
    }
    cpBodySetPositionFunc(body, cxChipmunkBodyUpdatePosition);
    cpBodySetVelocityFunc(body, cxChipmunkBodyUpdateVelocity);
    cpBodySetAngle(body, angle);
    cpBodySetPos(body, pos);
    cpBodySetUserData(body, nview);
    cpShape *shape = NULL;
    if(attr->shape == cxChipmunkShapeBox){
        shape = cpBoxShapeNew(body, size.w, size.h);
    }else if(attr->shape == cxChipmunkShapeCircle){
        shape = cpCircleShapeNew(body, r, attr->cp);
    }else{
        CX_ASSERT(false, "shape error");
    }
    cpShapeSetGroup(shape, attr->group);
    cpShapeSetCollisionType(shape, attr->ctype);
    cpShapeSetLayers(shape, attr->layer);
    cpShapeSetElasticity(shape, attr->e);
    cpShapeSetFriction(shape, attr->u);
    cpShapeSetUserData(shape, nview);
    cpSpaceAddShape(this->space, shape);
    cxViewSetArgs(nview, cxEventArgWeakRef(shape));
    cxViewAppend(this, nview);
    return nview;
}

void cxChipmunkRemove(cxAny pview,cxAny nview)
{
    cxChipmunk this = pview;
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(nview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    if(cpSpaceContainsBody(this->space, body)){
        cpSpaceRemoveBody(this->space, body);
    }
    cpBodyFree(body);
    cpSpaceRemoveShape(this->space, shape);
    cpShapeFree(shape);
    cxViewSetArgs(nview, NULL);
    cxViewRemoved(nview);
}

//free Chipmunk body shape
static void cxChipmunkFreeWrap(cpSpace *space, cpShape *shape, void *unused)
{
	cpSpaceRemoveShape(space, shape);
	cpShapeFree(shape);
}

static void cxChipmunkShapeFree(cpShape *shape, cpSpace *space)
{
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)cxChipmunkFreeWrap, shape, NULL);
}

static void cxChipmunkConstraintFreeWrap(cpSpace *space, cpConstraint *constraint, void *unused)
{
	cpSpaceRemoveConstraint(space, constraint);
	cpConstraintFree(constraint);
}

static void cxChipmunkConstraintFree(cpConstraint *constraint, cpSpace *space)
{
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)cxChipmunkConstraintFreeWrap, constraint, NULL);
}

static void cxChipmunkBodyFreeWrap(cpSpace *space, cpBody *body, void *unused)
{
    cxAny view = cpBodyGetUserData(body);
    cxViewRemoved(view);
	cpSpaceRemoveBody(space, body);
	cpBodyFree(body);
}

static void cxChipmunkBodyFree(cpBody *body, cpSpace *space)
{
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)cxChipmunkBodyFreeWrap, body, NULL);
}

CX_OBJECT_INIT(cxChipmunk, cxView)
{
    cxObjectSetReadAttrFunc(this, cxChipmunkReadAttr);
    this->space = cpSpaceNew();
    CX_EVENT_QUICK(this->super.onUpdate, cxChipmunkUpdate);
    cpSpaceSetGravity(this->space, cxVec2fv(0, -1000));
    cpSpaceSetUserData(this->space, this);
}
CX_OBJECT_FREE(cxChipmunk, cxView)
{
    cpSpaceEachShape(this->space, (cpSpaceShapeIteratorFunc)cxChipmunkShapeFree, this->space);
	cpSpaceEachConstraint(this->space, (cpSpaceConstraintIteratorFunc)cxChipmunkConstraintFree, this->space);
	cpSpaceEachBody(this->space, (cpSpaceBodyIteratorFunc)cxChipmunkBodyFree, this->space);
    cpSpaceFree(this->space);
}
CX_OBJECT_TERM(cxChipmunk, cxView)





