//
//  cxChipmunk.h
//  cxEngine
//
//  Created by xuhua on 11/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxChipmunk_h
#define cxEngine_cxChipmunk_h

#include <chipmunk/chipmunk.h>
#include <core/cxView.h>

CX_C_BEGIN

typedef enum {
    cxChipmunkShapeCircle = 1,
    cxChipmunkShapeBox,
}cxChipmunkShape;

typedef struct {
    cxChipmunkShape shape;
    cxVec2f cp;         //when shape == cxChipmunkShapeCircle
    cxBool isStatic;
    cxFloat m;
    cxFloat e;
    cxFloat u;
    cpGroup group;
    cpLayers layer;
    cpCollisionType ctype;
}cxChipmunkAttr;

CX_OBJECT_DEF(cxChipmunk, cxView)
    cpSpace *space;
CX_OBJECT_END(cxChipmunk)

cxChipmunkAttr *cxChipmunkAttrCreate();

void cxChipmunkReadAttr(cxReaderAttrInfo *info);

void cxChipmunkSetGravity(cxAny pview,cxVec2f gravity);

cxAny cxChipmunkAppend(cxAny pview,cxAny nview,cxChipmunkAttr *attr);

void cxChipmunkRemove(cxAny pview,cxAny nview);

void cxChipmunkSetPos(cxAny pview,cxVec2f pos);

void cxChipmunkSetAngle(cxAny pview,cxFloat angle);

void cxChipmunkSetElasticity(cxAny pview,cxFloat e);

void cxChipmunkSetFriction(cxAny pview,cxFloat u);

void cxChipmunkSetMass(cxAny pview,cxFloat m);

void cxChipmunkApplyForce(cxAny pview, cxVec2f force, cxVec2f r);

void cxChipmunkResetForces(cxAny pview);

void cxChipmunkApplyImpulse(cxAny pview, const cxVec2f j, const cxVec2f r);

void cxChipmunkSetCollisionType(cxAny pview,cxUInt type);

void cxChipmunkSetGroup(cxAny pview,cxUInt group);

void cxChipmunkSetLayers(cxAny pview,cxUInt layers);

CX_C_END

#endif
