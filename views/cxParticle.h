//
//  cxParticle.h
//  cxEngine
//
//  Created by xuhua on 10/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxParticle_h
#define cxEngine_cxParticle_h

#include <core/cxEventArg.h>
#include <core/cxNumber.h>
#include "cxAtlas.h"

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

#define MAX_RATE -1

CX_OBJECT_DEF(cxParticle, cxAtlas)
    cxBool isError;
    cxBool autoRemove;
    cxParticleEmitterType type;
    cxInt index;
    cxInt count;
    cxParticleUnit *units;
    cxBool isActive;
    cxFloat emitcounter;
    cxFloat elapsed;
    cxParticleBlendMode blend;
    cxInt number;
    cxFloat duration;
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
    CX_METHOD_ALLOC(void, UpdateBox, cxAny, cxBoxPoint *);
    CX_EVENT_ALLOC(onUpdate);
CX_OBJECT_END(cxParticle)

cxAny cxParticleCreate(cxConstChars texURL,cxInt number);

void cxParticleUpdate(cxAny pview);

cxParticle cxParticleCreateFromPEX(cxConstChars file);

void cxParticleInitFromPEX(cxAny pview,cxConstChars file);

void cxParticleSetType(cxAny pview,cxParticleEmitterType type);

void cxParticleSetBlendMode(cxAny pview,cxParticleBlendMode mode);

void cxParticleReadAttr(cxReaderAttrInfo *info);

void cxParticleStop(cxAny pview);

void cxParticleReset(cxAny pview);

void cxParticleInit(cxAny pview,cxInt number);

CX_C_END

#endif










