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

typedef struct {
    cxFloat angle;
    cxVec2f position;
    cxFloat life;
    cxFloat speed;
    cxParticleUnit *unit;
}cxParticleUnitInfo;

CX_DEF(cxParticle, cxAction)
    cxParticleEmitterType type;
    cxInt index;
    cxInt count;
    cxParticleUnit *units;
    cxBool isActive;
    cxFloat emitcounter;
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
    cxVec3f axisspin;
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
    //atlas index offset
    cxInt idxOffset;
CX_END(cxParticle, cxAction)

void cxParticleFireInitUnit(cxAny pav,cxParticleUnit *particle,cxInt index);

void cxParticleSetType(cxAny pav,cxParticleEmitterType type);

void cxParticleInitNumber(cxAny pav,cxInt number);

void cxParticleSetOffset(cxAny pav,cxInt offset);

CX_C_END

#endif
