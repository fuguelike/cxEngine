//
//  cxRunParticle.h
//  cxEngine
//  粒子
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxRunParticle_h
#define cxEngine_cxRunParticle_h

#include <engine/cxAction.h>
#include <views/cxAtlas.h>

CX_C_BEGIN

typedef enum {
    cxParticleEmitterGravity,
    cxParticleEmitterRadial,
}cxParticleEmitterType;

typedef struct {
    cxVec2f position;
    cxColor4f color;
    cxColor4f deltacolor;
    cxFloat size;
    cxFloat deltasize;
    cxFloat rotation;
    cxFloat deltarotation;
    cxFloat life;
    //gravity mode
    cxVec2f dir;
    cxFloat radaccel;
    cxFloat tanaccel;
    //radial mode
    cxFloat angle;
    cxFloat degreespers;
    cxFloat radius;
    cxFloat deltaradius;
}cxParticleUnit;

typedef enum {
    cxParticleBlendAdd,
    cxParticleBlendMultiply,
}cxParticleBlendMode;

CX_DEF(cxParticle, cxAction)
    cxFloat time;
    cxAtlas atlas;
    cxParticleEmitterType type;
    cxInt index;
    cxInt count;
    cxParticleUnit *units;
    cxBool isActive;
    cxFloat emitcounter;
    //base property
    cxParticleBlendMode blend;
    cxInt number;
    cxFloat rate;
    cxVec2fRange position;
    cxFloatRange life;
    cxFloatRange angle;
    cxFloatRange startsize;
    cxFloatRange endsize;
    cxColor4fRange startcolor;
    cxColor4fRange endcolor;
    cxFloatRange startspin;
    cxFloatRange endspin;
    //gravity mode
    cxVec2f gravity;
    cxBool todir;
    cxFloatRange speed;
    cxFloatRange tanaccel;
    cxFloatRange radaccel;
    //radial mode
    cxFloatRange startradius;
    cxFloatRange endradius;
    cxFloatRange rotatepers;
    //use multiple texture
    cxMemory boxtexs;
    //
    CX_METHOD_DEF(cxBoxTex2f *, GetBoxTex, cxAny, cxInt);
    CX_METHOD_DEF(cxAny, GetDrawView,cxAny);
    CX_METHOD_DEF(void, InitUnit,cxAny,cxParticleUnit *,cxInt);
    CX_SLOT_ALLOC(onDraw);
CX_END(cxParticle, cxAction)

cxBoxTex2f *cxParticleGetBoxTex(cxAny pav,cxInt index);

void cxParticleInitUnit(cxAny pav,cxParticleUnit *particle,cxInt index);

void cxParticleStop(cxAny pav);

void cxParticleSetType(cxAny pav,cxParticleEmitterType type);

void cxParticleInitNumber(cxAny pav,cxInt number);

void cxParticleSetBlendMode(cxAny pav,cxParticleBlendMode mode);

cxParticle cxParticleCreate(cxFloat time,cxConstChars url,cxInt number);

CX_C_END

#endif
