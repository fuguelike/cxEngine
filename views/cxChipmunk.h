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

void cxChipmunkSetPosition(cxAny pview,cxVec2f pos);

CX_C_END

#endif
