//
//  cxParticle.c
//  cxEngine
//
//  Created by xuhua on 10/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <streams/cxAssetsStream.h>
#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include <core/cxViewRoot.h>
#include "cxParticle.h"

static cxInt cxEmitterType(lua_State *L)
{
    cxNumber num = cxNumberInt(cxParticleEmitterGravity);
    if(!lua_isstring(L, 2)){
        CX_LUA_PUSH_OBJECT(num);
        return 1;
    }
    cxConstChars mode = lua_tostring(L, 2);
    if(cxConstCharsEqu(mode, "gravity")){
        num = cxNumberInt(cxParticleEmitterGravity);
    }else if(cxConstCharsEqu(mode, "radial")){
        num = cxNumberInt(cxParticleEmitterRadial);
    }
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxBlendMode(lua_State *L)
{
    cxNumber num = cxNumberInt(cxParticleBlendMultiply);
    if(!lua_isstring(L, 2)){
        CX_LUA_PUSH_OBJECT(num);
        return 1;
    }
    cxConstChars mode = lua_tostring(L, 2);
    if(cxConstCharsEqu(mode, "add")){
        num = cxNumberInt(cxParticleBlendAdd);
    }else if(cxConstCharsEqu(mode, "multiple")){
        num = cxNumberInt(cxParticleBlendMultiply);
    }
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

void __cxParticleTypeInit()
{
    cxEngineRegisteFunc(cxEmitterType);
    cxEngineRegisteFunc(cxBlendMode);
}

void cxParticleReadAttr(cxReaderAttrInfo *info)
{
    cxAtlasReadAttr(info);
    cxParticle this = info->object;
    cxInt number = cxXMLReadIntAttr(info, "cxParticle.number", this->number);
    CX_ASSERT(number > 0, "cxParticle number must > 0");
    cxParticleInit(this, number);
    cxParticleSetBlendMode(this, cxXMLReadIntAttr(info, "cxParticle.blend", cxParticleBlendMultiply));
    cxParticleSetType(this, cxXMLReadIntAttr(info, "cxParticle.type", cxParticleEmitterGravity));
    this->autoRemove    = cxXMLReadBoolAttr(info, "cxParticle.remove", this->autoRemove);
    this->todir         = cxXMLReadBoolAttr(info, "cxParticle.todir", this->todir);
    this->duration      = cxXMLReadFloatAttr(info, "cxParticle.duration", this->duration);
    this->gravity       = cxXMLReadVec2fAttr(info, "cxParticle.gravity", this->gravity);
    this->rate          = cxXMLReadFloatAttr(info, "cxParticle.rate", this->rate);
    this->speed         = cxXMLReadFloatRangeAttr(info, "cxParticle.speed", this->speed);
    this->tanaccel      = cxXMLReadFloatRangeAttr(info, "cxParticle.tanaccel", this->tanaccel);
    this->radaccel      = cxXMLReadFloatRangeAttr(info, "cxParticle.radaccel", this->radaccel);
    this->position      = cxXMLReadVec2fRangeAttr(info, "cxParticle.position", this->position);
    this->life          = cxXMLReadFloatRangeAttr(info, "cxParticle.life", this->life);
    this->angle         = cxXMLReadFloatRangeAttr(info, "cxParticle.angle", this->angle);
    this->startsize     = cxXMLReadFloatRangeAttr(info, "cxParticle.startsize", this->startsize);
    this->endsize       = cxXMLReadFloatRangeAttr(info, "cxParticle.endsize", this->endsize);
    this->startcolor    = cxXMLReadColor4fRangeAttr(info, "cxParticle.startcolor", this->startcolor);
    this->endcolor      = cxXMLReadColor4fRangeAttr(info, "cxParticle.endcolor", this->endcolor);
    this->startspin     = cxXMLReadFloatRangeAttr(info, "cxParticle.startspin", this->startspin);
    this->endspin       = cxXMLReadFloatRangeAttr(info, "cxParticle.endspin", this->endspin);
    this->startradius   = cxXMLReadFloatRangeAttr(info, "cxParticle.startradius", this->startradius);
    this->endradius     = cxXMLReadFloatRangeAttr(info, "cxParticle.endradius", this->endradius);
    this->rotatepers    = cxXMLReadFloatRangeAttr(info, "cxParticle.rotatepers", this->rotatepers);
}

cxAny cxParticleCreate(cxConstChars texURL,cxInt number)
{
    cxParticle this = CX_CREATE(cxParticle);
    cxParticleInit(this, number);
    cxSpriteSetTextureURL(this, texURL, true);
    cxParticleSetBlendMode(this, cxParticleBlendMultiply);
    return this;
}

void cxParticleStop(cxAny pview)
{
    cxParticle this = pview;
    this->isActive = false;
    this->emitcounter = 0;
    cxAtlasClean(pview);
}

void cxParticleReset(cxAny pview)
{
    cxParticle this = pview;
    this->isActive = true;
    this->emitcounter = 0;
    this->elapsed = 0;
    for(this->index = 0; this->index < this->count; this->index++){
        cxParticleUnit *p = &this->units[this->index];
        p->life = 0;
    }
    cxAtlasClean(pview);
}

static void cxParticleUnitToBoxVec3f(cxParticleUnit *particle,cxBoxVec3f *vq)
{
    cxFloat sizeh = particle->size/2;
    if (particle->rotation){
        cxFloat x1 = -sizeh;
        cxFloat y1 = -sizeh;
        cxFloat x2 = sizeh;
        cxFloat y2 = sizeh;
        cxFloat r = -kmDegreesToRadians(particle->rotation);
        cxFloat cr = cosf(r);
        cxFloat sr = sinf(r);
        vq->lb.x = x1 * cr - y1 * sr + particle->position.x;
        vq->lb.y = x1 * sr + y1 * cr + particle->position.y;
        vq->rb.x = x2 * cr - y1 * sr + particle->position.x;
        vq->rb.y = x2 * sr + y1 * cr + particle->position.y;
        vq->rt.x = x2 * cr - y2 * sr + particle->position.x;
        vq->rt.y = x2 * sr + y2 * cr + particle->position.y;
        vq->lt.x = x1 * cr - y2 * sr + particle->position.x;
        vq->lt.y = x1 * sr + y2 * cr + particle->position.y;
    }else{
        vq->lb.x = particle->position.x - sizeh;
        vq->lb.y = particle->position.y - sizeh;
        vq->rb.x = particle->position.x + sizeh;
        vq->rb.y = particle->position.y - sizeh;
        vq->lt.x = particle->position.x - sizeh;
        vq->lt.y = particle->position.y + sizeh;
        vq->rt.x = particle->position.x + sizeh;
        vq->rt.y = particle->position.y + sizeh;
    }
}

static void cxParticleStep(cxAny pview,cxParticleUnit *particle)
{
    cxParticle this = pview;
    cxBoxVec3f vq={0};
    cxColor4f color = particle->color;
    cxParticleUnitToBoxVec3f(particle, &vq);
    cxBoxPoint *box = &this->super.boxes[this->index];
    box->lb.colors = color;
    box->rb.colors = color;
    box->lt.colors = color;
    box->rt.colors = color;
    box->lb.texcoords = this->super.super.texCoord.lb;
    box->rb.texcoords = this->super.super.texCoord.rb;
    box->lt.texcoords = this->super.super.texCoord.lt;
    box->rt.texcoords = this->super.super.texCoord.rt;
    box->lb.vertices = vq.lb;
    box->rb.vertices = vq.rb;
    box->lt.vertices = vq.lt;
    box->rt.vertices = vq.rt;
    CX_METHOD_RUN(this->UpdateBox,this,box);
    this->super.number = CX_MAX(this->index + 1, this->super.number);
}

static void cxParticleInitUnit(cxAny pview,cxParticleUnit *particle)
{
    cxParticle this = pview;
    
    cxParticleInitArgs args={0};
    args.position = cxVec2fValue(this->position);
    args.angle = kmDegreesToRadians(cxFloatValue(this->angle));
    args.speed = cxFloatValue(this->speed);
    args = CX_METHOD_GET(args, this->InitArgs, this, &args);
    
    particle->life = cxFloatValue(this->life);
    particle->position = args.position;
    
    cxColor4f startcolor = cxColor4fValue(this->startcolor);
    cxColor4f endcolor = cxColor4fValue(this->endcolor);
    cxFloat startsize = cxFloatValue(this->startsize);
    cxFloat endsize = cxFloatValue(this->endsize);
    cxFloat startspin = cxFloatValue(this->startspin);
    cxFloat endspin = cxFloatValue(this->endspin);
    
    particle->color = startcolor;
    particle->deltacolor.r = (endcolor.r - startcolor.r) / particle->life;
    particle->deltacolor.g = (endcolor.g - startcolor.g) / particle->life;
    particle->deltacolor.b = (endcolor.b - startcolor.b) / particle->life;
    particle->deltacolor.a = (endcolor.a - startcolor.a) / particle->life;
    particle->size = startsize;
    particle->deltasize = (endsize - startsize) / particle->life;
    particle->rotation = startspin;
    particle->deltarotation = (endspin - startspin) / particle->life;
    
    if(this->type == cxParticleEmitterGravity){
        cxVec2f v = cxVec2fv(cosf(args.angle), sinf(args.angle));
        kmVec2Scale(&particle->dir, &v, args.speed);
        particle->radaccel = cxFloatValue(this->radaccel);
        particle->tanaccel = cxFloatValue(this->tanaccel);
        if(this->todir){
            particle->rotation = -kmRadiansToDegrees(cxVec2fAngle(particle->dir));
        }
    }else{
        cxFloat startradius = cxFloatValue(this->startradius);
        cxFloat endradius = cxFloatValue(this->endradius);
        particle->radius = startradius;
        if(endradius == -1){
            particle->deltaradius = 0;
        }else{
            particle->deltaradius = (endradius - startradius) / particle->life;
        }
        particle->angle = args.angle;
        particle->degreespers = kmDegreesToRadians(cxFloatValue(this->rotatepers));
    }
}

static cxBool cxParticleAdd(cxAny pview)
{
    cxParticle this = pview;
    if(this->count >= this->number){
        return false;
    }
    cxParticleUnit *unit = &this->units[this->count];
    cxParticleInitUnit(pview, unit);
    this->count ++;
    return true;
}

static void cxParticleComputeUnitVertex(cxParticle this,cxParticleUnit *p,cxFloat dt)
{
    if(this->type == cxParticleEmitterGravity){
        cxVec2f tmp;
        cxVec2f radial= cxVec2fv(0, 0);
        if(p->position.x || p->position.y){
            kmVec2Normalize(&radial, &p->position);
        }
        cxVec2f tangential = radial;
        kmVec2Scale(&radial, &radial, p->radaccel);
        //compute tangential
        float newy = tangential.x;
        tangential.x = -tangential.y;
        tangential.y = newy;
        kmVec2Scale(&tangential,&tangential,p->tanaccel);
        //compute position
        kmVec2Add(&tmp, &radial, &tangential);
        kmVec2Add(&tmp, &tmp, &this->gravity);
        kmVec2Scale(&tmp, &tmp, dt);
        kmVec2Add(&p->dir, &p->dir, &tmp);
        kmVec2Scale(&tmp, &p->dir, dt);
        kmVec2Add(&p->position, &p->position, &tmp);
    }else{
        p->angle += p->degreespers * dt;
        p->radius += p->deltaradius * dt;
        p->position.x = -cosf(p->angle) * p->radius;
        p->position.y = -sinf(p->angle) * p->radius;
    }
}

static void cxParticleComputeUnit(cxParticle this,cxParticleUnit *p,cxFloat dt)
{
    p->life -= dt;
    if(p->life > 0){
        //vertex
        cxParticleComputeUnitVertex(this, p, dt);
        // color
        p->color.r += (p->deltacolor.r * dt);
        p->color.g += (p->deltacolor.g * dt);
        p->color.b += (p->deltacolor.b * dt);
        p->color.a += (p->deltacolor.a * dt);
        //size
        p->size = CX_MAX(0, p->size + p->deltasize * dt);
        //angle
        p->rotation += (p->deltarotation * dt);
        cxParticleStep(this, p);
        this->index ++;
    }else{
        if(this->index != this->count - 1){
            this->units[this->index] = this->units[this->count - 1];
        }
        this->count--;
    }
}

static cxBool cxParticleComputeUnits(cxParticle this,cxFloat dt)
{
    if(!this->isActive){
        return false;
    }
    cxFloat rate = 1.0f / this->rate;
    if(this->count < this->number){
        this->emitcounter += dt;
    }
    while(this->count < this->number &&
          this->emitcounter > rate &&
          cxParticleAdd(this)){
        this->emitcounter -= rate;
    }
    this->elapsed += dt;
    if(this->duration == -1 || this->elapsed < this->duration){
        return false;
    }
    this->isActive = false;
    this->emitcounter = 0;
    cxAtlasClean(this);
    if(this->autoRemove){
        cxViewRemoved(this);
    }
    return true;
}

void cxParticleUpdate(cxAny pview)
{
    cxEngine engine = cxEngineInstance();
    cxFloat dt = engine->frameDelta;
    cxParticle this = pview;
    if(cxParticleComputeUnits(this, dt)){
        return;
    }
    CX_EVENT_FIRE(this, onUpdate);
    this->index = 0;
    while(this->index < this->count){
        cxParticleUnit *p = &this->units[this->index];
        cxParticleComputeUnit(this, p, dt);
    }
    this->super.isDirty = true;
}

static void cxParticleUpdateEvent(cxEvent *event)
{
    cxParticleUpdate(event->sender);
}

void cxParticleSetBlendMode(cxAny pview,cxParticleBlendMode mode)
{
    cxParticle this = pview;
    if(mode == cxParticleBlendAdd){
        cxSpriteSetBlendFactor(pview, GL_SRC_ALPHA, GL_ONE);
    }else{
        cxSpriteSetBlendFactor(pview, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    this->blend = mode;
}

void cxParticleSetType(cxAny pview,cxParticleEmitterType type)
{
    cxParticle this = pview;
    this->type = type;
}

CX_OBJECT_INIT(cxParticle, cxAtlas)
{
    this->rate = -1;
    this->duration = -1;
    this->isActive = true;
    this->type = cxParticleEmitterGravity;
    CX_EVENT_QUICK(this->super.super.super.onUpdate, cxParticleUpdateEvent);
    cxParticleSetBlendMode(this, cxParticleBlendAdd);
    cxObjectSetReadAttrFunc(this, cxParticleReadAttr);
}
CX_OBJECT_FREE(cxParticle, cxAtlas)
{
    allocator->free(this->units);
    CX_EVENT_RELEASE(this->onUpdate);
    CX_METHOD_RELEASE(this->UpdateBox);
    CX_METHOD_RELEASE(this->InitArgs);
}
CX_OBJECT_TERM(cxParticle, cxAtlas)

void cxParticleInit(cxAny pview,cxInt number)
{
    cxParticle this = pview;
    this->number = number;
    this->units = allocator->calloc(number,sizeof(cxParticleUnit));
    cxAtlasSetNumber(this, number);
    cxSetRandSeed();
}




