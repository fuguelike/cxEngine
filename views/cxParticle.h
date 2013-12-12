//
//  cxParticle.h
//  cxEngine
//
//  Created by xuhua on 10/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxParticle_h
#define cxEngine_cxParticle_h

#include "cxAtlas.h"

CX_C_BEGIN

typedef enum {
    cxParticleEmitterGravity,
    cxParticleEmitterRadial,
}cxParticleEmitterType;

typedef struct {
    cxInt index;
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

typedef void (*cxParticleUpdateBoxFunc)(cxAny pview,cxBoxPoint *box);

CX_OBJECT_DEF(cxParticle, cxAtlas)
    cxBool isError;
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
    CX_METHOD_DEF(cxParticleUpdateBoxFunc, UpdateBox);
CX_OBJECT_END(cxParticle)

void cxParticleInitFromPEX(cxAny pview,cxConstChars file);

void cxParticleSetType(cxAny pview,cxParticleEmitterType type);

void cxParticleSetBlendMode(cxAny pview,cxParticleBlendMode mode);

void cxParticleXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader);

void cxParticleStop(cxAny pview);

void cxParticleReset(cxAny pview);

void cxParticleInit(cxAny pview,cxInt number);

CX_C_END

#endif










