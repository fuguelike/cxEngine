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
    cxVec2f dir;
    cxFloat radaccel;
    cxFloat tanaccel;
}cxParticleUnit;

typedef struct {
    cxFloat v;  //base value
    cxFloat r;  //random value
}cxFloatRange;

typedef enum {
    cxParticleBlendAdd,
    cxParticleBlendMultiply,
}cxParticleBlendMode;

#define cxFloatValue(fv) ((fv).v + (fv).r * CX_RAND_11f())

#define cxFloatRangeValue(v,r) (cxFloatRange){v,r}

typedef struct {
    cxColor4f v;
    cxColor4f r;
}cxColorRange;

cxColor4f cxColorValue(cxColorRange r);

#define cxColorRangeValue(r1,g1,b1,a1,r2,g2,b2,a2)  (cxColorRange){{r1,g1,b1,a1},{r2,g2,b2,a2}}

typedef struct {
    cxVec2f v;
    cxVec2f r;
}cxVec2fRange;

cxVec2f cxVec2fValue(cxVec2fRange rv);

typedef void (*cxParticleUpdateBoxFunc)(cxAny pview,cxBoxPoint *box);

CX_OBJECT_DEF(cxParticle, cxAtlas)
    cxInt index;
    cxInt count;
    cxParticleUnit *units;
    cxBool isActive;
    cxFloat emitcounter;
    cxFloat elapsed;
    cxParticleBlendMode blend;
    cxInt number;
    cxFloat duration;
    cxVec2f gravity;
    cxBool todir;
    cxFloat rate;
    cxFloatRange speed;
    cxFloatRange tanaccel;
    cxFloatRange radaccel;
    cxVec2fRange position;
    cxFloatRange life;
    cxFloatRange angle;
    cxFloatRange startsize;
    cxFloatRange endsize;
    cxColorRange startcolor;
    cxColorRange endcolor;
    cxFloatRange startspin;
    cxFloatRange endspin;
    CX_METHOD_DEF(cxParticleUpdateBoxFunc, UpdateBox);
CX_OBJECT_END(cxParticle)

void cxParticleSetBlendMode(cxAny pview,cxParticleBlendMode mode);

void cxParticleXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader);

void cxParticleStop(cxAny pview);

void cxParticleReset(cxAny pview);

void cxParticleInit(cxAny pview,cxInt number);

CX_C_END

#endif










