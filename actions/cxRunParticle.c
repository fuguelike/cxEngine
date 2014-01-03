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

static cxInt cxRunParticleTypeImp(lua_State *L)
{
    cxNumber num = cxNumberInt(cxRunParticleTypeDraw);
    cxConstChars mode = luaL_checkstring(L, 1);
    if(cxConstCharsEqu(mode, "draw")){
        num = cxNumberInt(cxRunParticleTypeDraw);
    }else if(cxConstCharsEqu(mode, "after")){
        num = cxNumberInt(cxRunParticleTypeAfter);
    }else if(cxConstCharsEqu(mode, "before")){
        num = cxNumberInt(cxRunParticleTypeBefore);
    }
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

void cxRunParticleTypeInit()
{
    cxEngineRegisteNameFunc(cxRunParticleType,cxRunParticleTypeImp);
}

static void cxRunParticleDraw(cxAny pav)
{
    cxRunParticle this = pav;
    cxAtlasDraw(this->particleView);
}

static void cxRunParticleInit(cxAny pav)
{
    cxRunParticle this = pav;
    cxView view = this->super.view;
    cxParticle particle = this->particleView;
    CX_ASSERT(particle != NULL, "particle not set");
    cxParticleReset(particle);
    if(this->type == cxRunParticleTypeBefore){
        CX_SLOT_CONNECT(view->EmmitBefore, this, DrawParticle, cxRunParticleDraw);
    }else if(this->type == cxRunParticleTypeDraw){
        CX_SLOT_CONNECT(view->EmmitDraw, this, DrawParticle, cxRunParticleDraw);
    }else if(this->type == cxRunParticleTypeAfter){
        CX_SLOT_CONNECT(view->EmmitAfter, this, DrawParticle, cxRunParticleDraw);
    }else {
        CX_ASSERT(false, "cx run type error");
    }
}

static void cxRunParticleOver(cxAny pav)
{
    cxRunParticle this = pav;
    cxParticleStop(this->particleView);
    CX_SLOT_RELEASE(this->DrawParticle);
}

static void cxRunParticleStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxRunParticle this = pav;
    cxParticleUpdate(this->particleView);
}

static void cxRunParticleReadAttr(cxReaderAttrInfo *info)
{
    cxRunParticle this = info->object;
    cxActionReadAttr(info);
    cxConstChars pex = cxXMLAttr(info->reader, "cxRunParticle.pex");
    if(pex != NULL){
        cxParticle particle = cxParticleCreateFromPEX(pex);
        CX_RETAIN_SWAP(this->particleView, particle);
        this->super.duration = particle->duration;
        return;
    }
}

CX_OBJECT_INIT(cxRunParticle, cxAction)
{
    this->type = cxRunParticleTypeBefore;
    cxObjectSetReadAttrFunc(this, cxRunParticleReadAttr);
    CX_METHOD_OVERRIDE(this->super.Init, cxRunParticleInit);
    CX_METHOD_OVERRIDE(this->super.Over, cxRunParticleOver);
    CX_METHOD_OVERRIDE(this->super.Step, cxRunParticleStep);
}
CX_OBJECT_FREE(cxRunParticle, cxAction)
{
    CX_SLOT_RELEASE(this->DrawParticle);
    CX_RELEASE(this->particleView);
}
CX_OBJECT_TERM(cxRunParticle, cxAction)

cxRunParticle cxRunParticleCreateWithPEX(cxConstChars file,cxRunParticleType type)
{
    cxParticle v = cxParticleCreateFromPEX(file);
    return cxRunParticleCreate(v, type);
}

cxRunParticle cxRunParticleCreate(cxAny particleView,cxRunParticleType type)
{
    CX_ASSERT(particleView != NULL, "args error");
    cxParticle particle = particleView;
    cxRunParticle this = CX_CREATE(cxRunParticle);
    this->type = type;
    CX_RETAIN_SWAP(this->particleView, particleView);
    this->super.duration = particle->duration;
    return this;
}



