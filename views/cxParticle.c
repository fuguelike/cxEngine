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
    cxConstChars mode = luaL_checkstring(L, 1);
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
    cxConstChars mode = luaL_checkstring(L, 1);
    if(cxConstCharsEqu(mode, "add")){
        num = cxNumberInt(cxParticleBlendAdd);
    }else if(cxConstCharsEqu(mode, "multiple")){
        num = cxNumberInt(cxParticleBlendMultiply);
    }
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

void cxParticleTypeInit()
{
    cxEngineRegisteFunc(cxEmitterType);
    cxEngineRegisteFunc(cxBlendMode);
}

static void cxParticleXMLReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxParticle this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

cxParticle cxParticleCreateFromPEX(cxConstChars file)
{
    cxParticle this = CX_CREATE(cxParticle);
    cxParticleInitFromPEX(this, file);
    return this;
}

void cxParticleInitFromPEX(cxAny pview,cxConstChars file)
{
    cxParticle this = pview;
    cxXMLScript script = cxEngineGetXMLScript(file);
    CX_RETURN(script == NULL);
    xmlTextReaderPtr reader = cxXMLReaderForScript(script, cxParticleXMLReaderError, this);
    cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, NULL, this);
    while(xmlTextReaderRead(reader) && !this->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(ELEMENT_IS_TYPE(texture)){
            cxConstChars name = cxXMLAttr(reader, "name");
            cxSpriteSetTextureURL(this, name, true, true);
        }else if(ELEMENT_IS_TYPE(sourcePosition)){
            this->position.v.x = 0;//cxXMLReadFloatAttr(reader,NULL, "x", 0);
            this->position.v.y = 0;//cxXMLReadFloatAttr(reader,NULL, "y", 0);
        }else if(ELEMENT_IS_TYPE(sourcePositionVariance)){
            this->position.r.x = cxXMLReadFloatAttr(info, "x", 0);
            this->position.r.y = cxXMLReadFloatAttr(info, "y", 0);
        }else if(ELEMENT_IS_TYPE(speed)){
            this->speed.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if(ELEMENT_IS_TYPE(speedVariance)){
            this->speed.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if(ELEMENT_IS_TYPE(particleLifeSpan)){
            this->life.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if(ELEMENT_IS_TYPE(particleLifespanVariance)){
            this->life.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if(ELEMENT_IS_TYPE(angle)){
            this->angle.v = -cxXMLReadFloatAttr(info, "value", 0);
        }else if(ELEMENT_IS_TYPE(angleVariance)){
            this->angle.r = -cxXMLReadFloatAttr(info, "value", 0);
        }else if(ELEMENT_IS_TYPE(gravity)){
            this->gravity.x = cxXMLReadFloatAttr(info, "x", 0);
            this->gravity.y = cxXMLReadFloatAttr(info, "y", 0);
        }else if (ELEMENT_IS_TYPE(radialAcceleration)){
            this->radaccel.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(radialAccelVariance)){
            this->radaccel.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(tangentialAcceleration)){
            this->tanaccel.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(tangentialAccelVariance)){
            this->tanaccel.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(startColor)){
            this->startcolor.v.r = cxXMLReadFloatAttr(info, "red", 0);
            this->startcolor.v.g = cxXMLReadFloatAttr(info, "green", 0);
            this->startcolor.v.b = cxXMLReadFloatAttr(info, "blue", 0);
            this->startcolor.v.a = cxXMLReadFloatAttr(info, "alpha", 0);
        }else if (ELEMENT_IS_TYPE(startColorVariance)){
            this->startcolor.r.r = cxXMLReadFloatAttr(info, "red", 0);
            this->startcolor.r.g = cxXMLReadFloatAttr(info, "green", 0);
            this->startcolor.r.b = cxXMLReadFloatAttr(info, "blue", 0);
            this->startcolor.r.a = cxXMLReadFloatAttr(info, "alpha", 0);
        }else if (ELEMENT_IS_TYPE(finishColor)){
            this->endcolor.v.r = cxXMLReadFloatAttr(info, "red", 0);
            this->endcolor.v.g = cxXMLReadFloatAttr(info, "green", 0);
            this->endcolor.v.b = cxXMLReadFloatAttr(info, "blue", 0);
            this->endcolor.v.a = cxXMLReadFloatAttr(info, "alpha", 0);
        }else if (ELEMENT_IS_TYPE(finishColorVariance)){
            this->endcolor.r.r = cxXMLReadFloatAttr(info, "red", 0);
            this->endcolor.r.g = cxXMLReadFloatAttr(info, "green", 0);
            this->endcolor.r.b = cxXMLReadFloatAttr(info, "blue", 0);
            this->endcolor.r.a = cxXMLReadFloatAttr(info, "alpha", 0);
        }else if (ELEMENT_IS_TYPE(maxParticles)){
            cxInt number = cxXMLReadIntAttr(info, "value", 0);
            CX_ASSERT(number > 0, "particle number must > 0");
            cxParticleInit(this, number);
        }else if (ELEMENT_IS_TYPE(startParticleSize)){
            this->startsize.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(startParticleSizeVariance)){
            this->startsize.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(finishParticleSize)){
            this->endsize.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(FinishParticleSizeVariance)){
            this->endsize.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(duration)){
            this->duration = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(emitterType)){
            this->type = cxXMLReadIntAttr(info, "value", cxParticleEmitterGravity);
        }else if (ELEMENT_IS_TYPE(blendFuncSource)){
            this->super.super.sfactor = cxXMLReadIntAttr(info, "value", GL_SRC_ALPHA);
        }else if (ELEMENT_IS_TYPE(blendFuncDestination)){
            this->super.super.dfactor = cxXMLReadIntAttr(info, "value", GL_ONE_MINUS_SRC_ALPHA);
        }else if (ELEMENT_IS_TYPE(emitterRate)){
            this->rate = cxXMLReadFloatAttr(info, "value", this->rate);
        }else if (ELEMENT_IS_TYPE(particleToDir)){
            this->todir = cxXMLReadBoolAttr(info, "value", this->todir);
        }else if (ELEMENT_IS_TYPE(rotatePerSecond)){
            this->rotatepers.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if(ELEMENT_IS_TYPE(rotatePerSecondVariance)){
            this->rotatepers.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(rotationStart)){
            this->startspin.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(rotationStartVariance)){
            this->startspin.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(rotationEnd)){
            this->endspin.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(rotationEndVariance)){
            this->endspin.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(maxRadius)){
            this->startradius.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(maxRadiusVariance)){
            this->startradius.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(minRadius)){
            this->endradius.v = cxXMLReadFloatAttr(info, "value", 0);
        }else if (ELEMENT_IS_TYPE(minRadiusVariance)){
            this->endradius.r = cxXMLReadFloatAttr(info, "value", 0);
        }else if(ELEMENT_IS_TYPE(remove)){
            this->autoRemove = cxXMLReadBoolAttr(info, "value", this->autoRemove);
        }
    }
}

void cxParticleReadAttr(cxReaderAttrInfo *info)
{
    cxAtlasReadAttr(info);
    cxParticle this = info->object;
    cxConstChars pex = cxXMLAttr(info->reader, "cxParticle.pex");
    if(pex != NULL){
        cxParticleInitFromPEX(this, pex);
        return;
    }
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
    for(this->index = 0; this->index < this->count; ++this->index){
        cxParticleUnit *p = &this->units[this->index];
        p->life = 0;
    }
    this->position.r = cxVec2fv(0, 0);
    this->position.v = cxVec2fv(0, 0);
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
    CX_METHOD_FIRE(NULL, this->UpdateBox,this,box);
    this->super.number = CX_MAX(this->index + 1, this->super.number);
}

static void cxParticleInitUnit(cxAny pview,cxParticleUnit *particle)
{
    cxParticle this = pview;
    particle->life = cxFloatValue(this->life);
    particle->position = cxVec2fValue(this->position);
    
    cxColor4f startcolor = cxColor4fValue(this->startcolor);
    cxColor4f endcolor = cxColor4fValue(this->endcolor);
    
    particle->color = startcolor;
    particle->deltacolor.r = (endcolor.r - startcolor.r) / particle->life;
    particle->deltacolor.g = (endcolor.g - startcolor.g) / particle->life;
    particle->deltacolor.b = (endcolor.b - startcolor.b) / particle->life;
    particle->deltacolor.a = (endcolor.a - startcolor.a) / particle->life;
    
    cxFloat startsize = cxFloatValue(this->startsize);
    particle->size = startsize;
    cxFloat endsize = cxFloatValue(this->endsize);
    particle->deltasize = (endsize - startsize) / particle->life;
    
    cxFloat startspin = cxFloatValue(this->startspin);
    cxFloat endspin = cxFloatValue(this->endspin);
    particle->rotation = startspin;
    particle->deltarotation = (endspin - startspin) / particle->life;
    
    cxFloat angle = kmDegreesToRadians(cxFloatValue(this->angle));
    
    if(this->type == cxParticleEmitterGravity){
        cxVec2f v = cxVec2fv(cosf(angle), sinf(angle));
        cxFloat speed = cxFloatValue(this->speed);
        kmVec2Scale(&particle->dir, &v, speed);
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
        particle->angle = angle;
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
    unit->index = this->count;
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
        // size
        p->size = CX_MAX(0, p->size + p->deltasize * dt);
        // angle
        p->rotation += (p->deltarotation * dt);
        cxParticleStep(this,p);
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
    while(this->count < this->number && this->emitcounter > rate && cxParticleAdd(this)){
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
    cxViewOnUpdate(this, cxParticleUpdateEvent);
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE);
    cxObjectSetReadAttrFunc(this, cxParticleReadAttr);
}
CX_OBJECT_FREE(cxParticle, cxAtlas)
{
    allocator->free(this->units);
    CX_EVENT_RELEASE(this->onUpdate);
    CX_METHOD_RELEASE(this->UpdateBox);
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




