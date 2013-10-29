//
//  cxParticle.c
//  cxEngine
//
//  Created by xuhua on 10/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include "cxParticle.h"

cxBool cxParticleXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxAtlasXMLReadAttr(xmlView, mView, reader);
    cxParticle this = mView;
    cxInt number = cxXMLReadIntAttr(reader, "cxParticle.number", this->number);
    CX_ASSERT(number > 0, "cxParticle number must > 0");
    cxParticleInit(this, number);
    
    //mode add multiply
    cxChar *smode = cxXMLAttr("cxParticle.blend");
    if(strcasecmp(smode, "add") == 0){
        this->blend = cxParticleBlendAdd;
    }else if(strcasecmp(smode, "multiple") == 0){
        this->blend = cxParticleBlendMultiply;
    }
    xmlFree(smode);
    //
    this->todir = cxXMLReadBoolAttr(reader, "cxParticle.todir", this->todir);
    cxXMLReadFloatsAttr(reader, "cxParticle.duration", &this->duration);
    cxXMLReadFloatsAttr(reader, "cxParticle.gravity", &this->gravity.x);
    cxXMLReadFloatsAttr(reader, "cxParticle.rate", &this->rate);
    cxXMLReadFloatsAttr(reader, "cxParticle.speed", &this->speed.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.tanaccel", &this->tanaccel.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.radaccel", &this->radaccel.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.position", &this->position.v.x);
    cxXMLReadFloatsAttr(reader, "cxParticle.life", &this->life.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.angle", &this->angle.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.startsize", &this->startsize.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.endsize", &this->endsize.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.startcolor", &this->startcolor.v.r);
    cxXMLReadFloatsAttr(reader, "cxParticle.endcolor", &this->endcolor.v.r);
    cxXMLReadFloatsAttr(reader, "cxParticle.startspin", &this->startspin.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.endspin", &this->endspin.v);
    cxParticleSetBlendMode(this, this->blend);
    return true;
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
    CX_METHOD_RUN(this->UpdateBox,this,box);
    this->super.number = CX_MAX(this->index, this->super.number);
}

static void cxParticleInitUnit(cxAny pview,cxParticleUnit *particle)
{
    cxParticle this = pview;
    particle->life = cxFloatValue(this->life);
    particle->position = cxVec2fValue(this->position);
    cxColor4f startcolor = cxColorValue(this->startcolor);
    cxColor4f endcolor = cxColorValue(this->endcolor);
    
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
    cxVec2f v = cxVec2fv(cosf(angle), sinf(angle));
    cxFloat speed = cxFloatValue(this->speed);
    kmVec2Scale(&particle->dir, &v, speed);
    particle->radaccel = cxFloatValue(this->radaccel);
    particle->tanaccel = cxFloatValue(this->tanaccel);
    if(this->todir){
        particle->rotation = -kmRadiansToDegrees(cxVec2fAngle(particle->dir));
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

static void cxParticleUpdate(cxAny pview,cxAny arg)
{
    cxEngine engine = cxEngineInstance();
    cxFloat dt = engine->frameDelta;
    cxParticle this = pview;
    if(this->isActive){
        cxFloat rate = 1.0f / this->rate;
        if(this->count < this->number){
            this->emitcounter += dt;
        }
        while(this->count < this->number && this->emitcounter > rate && cxParticleAdd(pview)){
            this->emitcounter -= rate;
        }
        this->elapsed += dt;
        if(this->duration != -1 && this->elapsed > this->duration){
            this->isActive = false;
            this->emitcounter = 0;
            cxAtlasClean(this);
            return;
        }
    }
    this->index = 0;
    while(this->index < this->count){
        cxParticleUnit *p = &this->units[this->index];
        p->life -= dt;
        if(p->life > 0){
            cxVec2f tmp, radial, tangential;
            radial= cxVec2fv(0, 0);
            if(p->position.x || p->position.y){
                kmVec2Normalize(&radial, &p->position);
            }
            tangential = radial;
            kmVec2Scale(&radial, &radial, p->radaccel);
            //
            float newy = tangential.x;
            tangential.x = -tangential.y;
            tangential.y = newy;
            kmVec2Scale(&tangential,&tangential,p->tanaccel);
            //
            kmVec2Add(&tmp, &radial, &tangential);
            kmVec2Add(&tmp, &tmp, &this->gravity);
            kmVec2Scale(&tmp, &tmp, dt);
            kmVec2Add(&p->dir, &p->dir, &tmp);
            kmVec2Scale(&tmp, &p->dir, dt);
            kmVec2Add(&p->position, &p->position, &tmp);
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
    this->super.isDirty = true;
}

void cxParticleSetBlendMode(cxAny pview,cxParticleBlendMode mode)
{
    if(mode == cxParticleBlendAdd){
        cxSpriteSetBlendFactor(pview, GL_SRC_ALPHA, GL_ONE);
    }else{
        cxSpriteSetBlendFactor(pview, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

CX_OBJECT_INIT(cxParticle, cxAtlas)
{
    this->duration = -1;
    this->isActive = true;
    cxViewOnUpdate(this, cxParticleUpdate, NULL);
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE);
    cxObjectSetXMLReadFunc(this, cxParticleXMLReadAttr);
}
CX_OBJECT_FREE(cxParticle, cxAtlas)
{
    allocator->free(this->units);
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

cxColor4f cxColorValue(cxColorRange cv)
{
    cxColor4f color;
    color.r = kmClamp(cv.v.r + cv.r.r * CX_RAND_11f(), 0.0f, 1.0f);
    color.g = kmClamp(cv.v.g + cv.r.g * CX_RAND_11f(), 0.0f, 1.0f);
    color.b = kmClamp(cv.v.b + cv.r.b * CX_RAND_11f(), 0.0f, 1.0f);
    color.a = kmClamp(cv.v.a + cv.r.a * CX_RAND_11f(), 0.0f, 1.0f);
    return color;
}

cxVec2f cxVec2fValue(cxVec2fRange rv)
{
    cxVec2f pos;
    pos.x = rv.v.x + rv.r.x * CX_RAND_11f();
    pos.y = rv.v.y + rv.r.y * CX_RAND_11f();
    return pos;
}


