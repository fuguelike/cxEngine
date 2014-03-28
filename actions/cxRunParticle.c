//
//  cxRunParticle.c
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <views/cxParticle.h>
#include "cxRunParticle.h"

void __cxRunParticleTypeInit()
{
    
}

static void cxActionViewDraw(cxAny pav)
{
    cxRunParticle this = pav;
    cxAtlasDraw(this->particleView);
}

static void cxRunParticleInit(cxAny pav)
{
    cxRunParticle this = pav;
    cxView view = (this->drawView != NULL) ? this->drawView : cxActionView(pav);
    cxParticle particle = this->particleView;
    CX_ASSERT(particle != NULL, "particle not set");
    cxParticleReset(particle);
    CX_SLOT_CONNECT(view->onDraw, this, onDraw, cxActionViewDraw);
}

static void cxRunParticleOver(cxAny pav)
{
    cxRunParticle this = pav;
    cxParticleStop(this->particleView);
    CX_SLOT_RELEASE(this->onDraw);
}

static void cxRunParticleStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxRunParticle this = pav;
    cxParticleUpdate(this->particleView);
}

static void cxRunParticleReadAttr(cxReaderAttrInfo *info)
{
    cxActionReadAttr(info);
}

CX_OBJECT_INIT(cxRunParticle, cxAction)
{
    cxObjectSetReadAttrFunc(this, cxRunParticleReadAttr);
    CX_METHOD_OVERRIDE(this->super.Init, cxRunParticleInit);
    CX_METHOD_OVERRIDE(this->super.Over, cxRunParticleOver);
    CX_METHOD_OVERRIDE(this->super.Step, cxRunParticleStep);
}
CX_OBJECT_FREE(cxRunParticle, cxAction)
{
    CX_RELEASE(this->drawView);
    CX_SLOT_RELEASE(this->onDraw);
    CX_RELEASE(this->particleView);
}
CX_OBJECT_TERM(cxRunParticle, cxAction)

void cxRunParticleSetDrawView(cxAny pav,cxAny dv)
{
    cxRunParticle this = pav;
    CX_RETAIN_SWAP(this->drawView, dv);
}

cxRunParticle cxRunParticleCreate(cxAny particleView)
{
    CX_ASSERT(particleView != NULL, "args error");
    cxParticle particle = particleView;
    cxRunParticle this = CX_CREATE(cxRunParticle);
    CX_RETAIN_SWAP(this->particleView, particleView);
    this->super.duration = particle->duration;
    return this;
}



