//
//  cxChipmunk.h
//  cxEngine
//
//  Created by xuhua on 11/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxChipmunk_h
#define cxEngine_cxChipmunk_h

#include <core/cxView.h>
#include <chipmunk/chipmunk.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxChipmunk, cxView)
    cpSpace *space;
CX_OBJECT_END(cxChipmunk)

cxBool cxChipmunkXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader);

void cxChipmunkSetGravity(cxAny pview,cxVec2f gravity);

void cxChipmunkAppendAfter(cxAny pview,cxAny nview);

void cxChipmunkRemoveBefore(cxAny pview,cxAny nview);

void cxViewSetElasticity(cxAny pview,cxFloat e);

void cxViewSetFriction(cxAny pview,cxFloat u);

void cxViewSetMass(cxAny pview,cxFloat m);

void cxViewSetMoment(cxAny pview,cxFloat i);

void cxViewApplyForce(cxAny pview, cxVec2f force, cxVec2f r);

void cxViewResetForces(cxAny pview);

void cxViewApplyImpulse(cxAny pview, const cxVec2f j, const cxVec2f r);

void cxViewSetCollisionType(cxAny pview,cxUInt type);

void cxViewSetGroup(cxAny pview,cxUInt group);

void cxViewSetLayers(cxAny pview,cxUInt layers);

CX_C_END

#endif
