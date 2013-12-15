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

static cxChipmunkAttr cxChipmunkAttrDefault()
{
    cxChipmunkAttr attr = {0};
    attr.cp = cxVec2fv(0, 0);
    attr.ctype = 0;
    attr.e = 0.0f;;
    attr.group = CP_NO_GROUP;
    attr.isStatic = false;
    attr.layer = CP_ALL_LAYERS;
    attr.m = 1.0f;;
    attr.shape = cxChipmunkShapeBox;
    attr.u = 0.0f;
    return attr;
}
//cxShapeType('box')
static cxNumber cxChipmunkShapeType(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars shape = cxEventArgToString(arg);
    if(cxConstCharsEqu(shape, "box")){
        return cxNumberInt(cxChipmunkShapeBox);
    }
    if(cxConstCharsEqu(shape, "circle")){
        return cxNumberInt(cxChipmunkShapeCircle);
    }
    return cxNumberInt(cxChipmunkShapeBox);
}

void cxChipmunkTypeInit()
{
    cxEngineRegisteTypeFunc(cxChipmunkTypeName, "cxShapeType", cxChipmunkShapeType);
}

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

static cxChipmunkAttr cxChipmunkGetAttr(cxViewRoot root,xmlTextReaderPtr reader)
{
    cxChipmunkAttr attr = cxChipmunkAttrDefault();
    attr.shape = cxXMLReadIntAttr(reader, root->functions, "cxChipmunk.shape", cxChipmunkShapeBox);
    attr.cp = cxXMLReadVec2fAttr(reader, root->functions, "cxChipmunk.center", cxVec2fv(0, 0));
    attr.isStatic = cxXMLReadBoolAttr(reader,root->functions, "cxChipmunk.static", attr.isStatic);
    attr.m = cxXMLReadFloatAttr(reader, root->functions, "cxChipmunk.m", attr.m);
    attr.e = cxXMLReadFloatAttr(reader, root->functions, "cxChipmunk.e", attr.e);
    attr.u = cxXMLReadFloatAttr(reader, root->functions, "cxChipmunk.u", attr.u);
    attr.group = cxXMLReadUIntAttr(reader,root->functions, "cxChipmunk.group", CP_NO_GROUP);
    attr.layer = cxXMLReadUIntAttr(reader,root->functions, "cxChipmunk.layer", CP_ALL_LAYERS);
    attr.ctype = cxXMLReadUIntAttr(reader,root->functions, "cxChipmunk.ctype", 0);
    return attr;
}

void cxChipmunkReadAttr(cxAny rootView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewRoot root = rootView;
    cxViewReadAttr(rootView, mView, reader);
    cxChipmunk this = mView;
    //set space
    cpVect gravity = cpSpaceGetGravity(this->space);
    cxVec2f v = cxVec2fv(gravity.x, gravity.y);
    v = cxXMLReadVec2fAttr(reader, root->functions, "cxChipmunk.gravity", v);
    cpSpaceSetGravity(this->space, cpv(v.x, v.y));
    //load subview
    cxStack stack = CX_ALLOC(cxStack);
    cxStackPush(stack, mView);
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        int type = xmlTextReaderNodeType(reader);
        if(type == XML_READER_TYPE_ELEMENT){
            cxConstChars temp = cxXMLReadElementName(reader);
            CX_ASSERT(temp != NULL, "temp read error");
            cxView cview = cxViewRootLoadSubviewBegin(root, temp, reader);
            cxAny parent = cxStackTop(stack);
            if(parent == mView){
                cxChipmunkAttr attr = cxChipmunkGetAttr(root, reader);
                cxChipmunkAppend(parent, cview, &attr);
            }else{
                cxViewAppend(parent, cview);
            }
            if(xmlTextReaderIsEmptyElement(reader)){
                continue;
            }
            cxStackPush(stack, cview);
        }else if(type == XML_READER_TYPE_END_ELEMENT){
            cxStackPop(stack);
        }
    }
    CX_RELEASE(stack);
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

void cxChipmunkSetRadians(cxAny pview,cxFloat angle)
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

cxAny cxChipmunkAppend(cxAny pview,cxAny nview,cxChipmunkAttr *attr)
{
    cxChipmunk this = pview;
    CX_ASSERT(cxViewArgs(nview) == NULL,"not set args's view can append");
    
    cxHashKey key = cxHashIntKey((cxInt)nview);
    cxSize2f size = cxViewSize(nview);
    CX_RETURN(cxSize2Zero(size), NULL);
    
    cxVec2f pos = cxViewPosition(nview);
    cxFloat angle = cxViewAngle(nview);
    cpBody *body = NULL;
    //get body attr
    cxFloat i = 0;
    cxFloat r = CX_MAX(size.w, size.h) / 2.0f;
    if(attr->shape == cxChipmunkShapeBox){
        i = cpMomentForBox(attr->m, size.w, size.h);
    }else if(attr->shape == cxChipmunkShapeCircle){
        i = cpMomentForCircle(attr->m, 0, r, attr->cp);
    }else{
        CX_ASSERT(false, "shape error");
    }
    //set body attr
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
    //set shape type
    cpShape *shape = NULL;
    if(attr->shape == cxChipmunkShapeBox){
        shape = cpBoxShapeNew(body, size.w, size.h);
    }else if(attr->shape == cxChipmunkShapeCircle){
        shape = cpCircleShapeNew(body, r, attr->cp);
    }else{
        CX_ASSERT(false, "shape error");
    }
    //
    cpShapeSetGroup(shape, attr->group);
    cpShapeSetCollisionType(shape, attr->ctype);
    cpShapeSetLayers(shape, attr->layer);
    cpShapeSetElasticity(shape, attr->e);
    cpShapeSetFriction(shape, attr->u);
    cpShapeSetUserData(shape, nview);
    cpSpaceAddShape(this->space, shape);
    //save shape
    cxViewSetArgs(nview, cxEventArgWeakRef(shape));
    cxViewAppend(this, nview);
    cxHashSet(this->items, key, nview);
    return nview;
}

static void cxChipmunkRemoveImp(cxAny pview,cxAny nview)
{
    cxChipmunk this = pview;
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(nview));
    CX_ASSERT(shape != NULL, "not add to cxChipmunk view");
    
    cpBody *body = cpShapeGetBody(shape);
    if(cpSpaceContainsBody(this->space, body)){
        cpSpaceRemoveBody(this->space, body);
    }
    cpBodyFree(body);
    
    cpSpaceRemoveShape(this->space, shape);
    cpShapeFree(shape);
    cxViewSetArgs(nview, NULL);
}

void cxChipmunkRemove(cxAny pview,cxAny nview)
{
    cxChipmunk this = pview;
    cxChipmunkRemoveImp(pview, nview);
    cxHashDel(this->items, cxHashIntKey((cxInt)nview));
}

CX_OBJECT_INIT(cxChipmunk, cxView)
{
    this->items = CX_ALLOC(cxHash);
    cxObjectSetReadAttrFunc(this, cxChipmunkReadAttr);
    this->space = cpSpaceNew();
    CX_EVENT_QUICK(this->super.onUpdate, cxChipmunkUpdate);
    cpSpaceSetGravity(this->space, cxVec2fv(0, -1000));
}
CX_OBJECT_FREE(cxChipmunk, cxView)
{
    CX_HASH_FOREACH(this->items, ele, tmp){
        cxChipmunkRemoveImp(this, ele->any);
    }
    CX_RELEASE(this->items);
    cpSpaceFree(this->space);
}
CX_OBJECT_TERM(cxChipmunk, cxView)





