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

void cxChipmunkXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewXML xml = xmlView;
    cxViewXMLReadAttr(xmlView, mView, reader);
    cxChipmunk this = mView;
    cpVect gravity = this->space->gravity;
    cxXMLReadFloatsAttr(reader, "cxChipmunk.gravity", &gravity.x);
    cpSpaceSetGravity(this->space, gravity);
    //load subview
    cxStack stack = CX_ALLOC(cxStack);
    cxStackPush(stack, mView);
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        int type = xmlTextReaderNodeType(reader);
        if(type == XML_READER_TYPE_ELEMENT){
            cxView parent = cxStackTop(stack);
            const xmlChar *temp = xmlTextReaderConstName(reader);
            cxView cview = CX_METHOD_GET(NULL, xml->Make, temp, reader);
            CX_ASSERT(cview != NULL, "make element null");
            cxObjectSetRoot(cview, xml);
            cxObjectXMLReadAttrRun(cview, xml, reader);
            cxViewXMLSet(xml, cview, reader);
            if(parent == mView){
                cxChipmunkAttr attr = cxChipmunkAttrDefault();
                attr.isStatic = cxXMLReadBoolAttr(reader, "cxChipmunk.static", attr.isStatic);
                attr.m = cxXMLReadFloatAttr(reader, "cxChipmunk.m", attr.m);
                attr.e = cxXMLReadFloatAttr(reader, "cxChipmunk.e", attr.e);
                attr.u = cxXMLReadFloatAttr(reader, "cxChipmunk.u", attr.u);
                attr.group = cxXMLReadUIntAttr(reader, "cxChipmunk.group", CP_NO_GROUP);
                attr.layer = cxXMLReadUIntAttr(reader, "cxChipmunk.layer", CP_ALL_LAYERS);
                attr.ctype = cxXMLReadUIntAttr(reader, "cxChipmunk.ctype", 0);
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

static void cxChipmunkUpdatePosition(cpBody *body, cpFloat dt)
{
    cpBodyUpdatePosition(body, dt);
    cxViewSetPos(body->data, cxVec2fv(body->p.x, body->p.y));
    cxViewSetRadians(body->data, body->a);
}

void cxChipmunkSetPos(cxAny pview,cxVec2f pos)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    CX_RETURN(cpBodyIsStatic(body));
    cpBodySetPos(body, cpv(pos.x, pos.y));
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
    cpBodyApplyForce(body, cpv(force.x, force.y), cpv(r.x, r.y));
}

void cxChipmunkApplyImpulse(cxAny pview, const cxVec2f j, const cxVec2f r)
{
    CX_ASSERT(pview != NULL, "view null");
    cpShape *shape = cxEventArgToWeakRef(cxViewArgs(pview));
    CX_RETURN(shape == NULL);
    cpBody *body = cpShapeGetBody(shape);
    cpBodyApplyImpulse(body, cpv(j.x, j.y), cpv(r.x, r.y));
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

cxAny cxChipmunkAppend(cxAny pview,cxAny nview,cxChipmunkAttr *attr)
{
    cxChipmunk this = pview;
    CX_ASSERT(cxViewArgs(nview) == NULL,"not set args's view can append");
    
    cxHashKey key = cxHashIntKey((cxInt)nview);
    cxSize2f size = cxViewSize(nview);
    CX_RETURN(cxSize2Zero(size), NULL);
    
    cxVec2f pos = cxViewPosition(nview);
    cxFloat radians = cxViewRadians(nview);
    //set body attr
    cpBody *body = NULL;
    if(attr->isStatic){
        body = cpBodyNewStatic();
    }else{
        body = cpBodyNew(attr->m, cpMomentForBox(attr->m, size.w, size.h));
        cpSpaceAddBody(this->space, body);
    }
    body->position_func = cxChipmunkUpdatePosition;
    cpBodySetAngle(body, radians);
    cpBodySetPos(body, cpv(pos.x, pos.y));
    cpBodySetUserData(body, nview);
    //set shao attr
    cpShape *shape = cpBoxShapeNew(body, size.w, size.h);
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
    cxObjectSetXMLReadFunc(this, cxChipmunkXMLReadAttr);
    this->space = cpSpaceNew();
    CX_EVENT_QUICK(this->super.onUpdate, cxChipmunkUpdate);
    cpSpaceSetGravity(this->space, cpv(0, -1000));
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





