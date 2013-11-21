//
//  cxChipmunk.c
//  cxEngine
//
//  Created by xuhua on 11/21/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxChipmunk.h"

static void cxChipmunkUpdate(cxEvent *event)
{
    cxEngine engine = cxEngineInstance();
    cxChipmunk this = event->sender;
    int steps = 2;
    for(int i=0; i<steps; i++){
        cpSpaceStep(this->space, engine->frameDelta);
    }
}

void cxChipmunkSetGravity(cxAny pview,cxVec2f gravity)
{
    cxChipmunk this = pview;
    cpSpaceSetGravity(this->space, cpv(gravity.x, gravity.y));
}

CX_OBJECT_INIT(cxChipmunk, cxView)
{
    this->space = cpSpaceNew();
    CX_EVENT_QUICK(this->super.onUpdate, cxChipmunkUpdate);
}
CX_OBJECT_FREE(cxChipmunk, cxView)
{
    cpSpaceFree(this->space);
}
CX_OBJECT_TERM(cxChipmunk, cxView)
