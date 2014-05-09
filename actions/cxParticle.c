//
//  cxParticle.c
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxParticle.h"

static void cxActionViewDraw(cxAny pav)
{
    cxParticle this = pav;
    cxAtlasDraw(this->atlas);
}

static void cxParticleInit(cxAny pav)
{
    cxParticle this = pav;
    cxView view = cxActionView(pav);
    view = CX_METHOD_GET(view, this->GetDrawView, this);
    CX_ASSERT(view != NULL, "draw view must not null");
    CX_SLOT_CONNECT(view->onDraw, this, onDraw, cxActionViewDraw);
}

static void cxParticleOver(cxAny pav)
{
    cxParticle this = pav;
    this->isActive = false;
    this->emitcounter = 0;
    cxAtlasClean(this->atlas);
    CX_SLOT_RELEASE(this->onDraw);
}

static void cxParticleInitUnit(cxParticle this,cxParticleUnit *particle)
{
    cxParticleArgs args = {0};
    args.position = cxVec2fValue(this->position);
    args.angle = kmDegreesToRadians(cxFloatValue(this->angle));
    args.speed = cxFloatValue(this->speed);
    CX_METHOD_RUN(this->SetUnitArgs, this, &args);
    
    particle->position = args.position;
    particle->life = cxFloatValue(this->life);
    
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

static cxBool cxParticleAdd(cxParticle this)
{
    if(this->count >= this->number){
        return false;
    }
    cxParticleUnit *unit = &this->units[this->count];
    cxParticleInitUnit(this, unit);
    this->count ++;
    return true;
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

static void cxParticleSetBox(cxAny pav,cxParticleUnit *particle)
{
    cxParticle this = pav;
    cxBoxVec3f vq={0};
    cxColor4f color = particle->color;
    cxParticleUnitToBoxVec3f(particle, &vq);
    
    cxBoxPoint *box = &this->atlas->boxes[this->index];
    box->lb.colors = color;
    box->rb.colors = color;
    box->lt.colors = color;
    box->rt.colors = color;
    box->lb.texcoords = this->atlas->super.texCoord.lb;
    box->rb.texcoords = this->atlas->super.texCoord.rb;
    box->lt.texcoords = this->atlas->super.texCoord.lt;
    box->rt.texcoords = this->atlas->super.texCoord.rt;
    box->lb.vertices = vq.lb;
    box->rb.vertices = vq.rb;
    box->lt.vertices = vq.lt;
    box->rt.vertices = vq.rt;
}

void cxParticleStop(cxAny pav)
{
    cxParticle this = pav;
    this->isActive = false;
    this->emitcounter = 0;
}

static void cxParticleStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxParticle this = pav;
    if(this->isActive){
        cxFloat rate = 1.0f / this->rate;
        if(this->count < this->number){
            this->emitcounter += dt;
        }
        while(this->count < this->number && this->emitcounter > rate){
            cxParticleAdd(this);
            this->emitcounter -= rate;
        }
		if (this->duration != -1 && this->duration < this->super.durationElapsed){
			cxParticleStop(pav);
        }
    }
    this->index = 0;
    while(this->index < this->count){
        cxParticleUnit *p = &this->units[this->index];
        p->life -= dt;
        if(p->life > 0){
            //vertex
            if(this->type == cxParticleEmitterGravity){
                cxVec2f tmp;
                cxVec2f radial= cxVec2fv(0, 0);
                if(p->position.x || p->position.y){
                    kmVec2Normalize(&radial, &p->position);
                }
                cxVec2f tangential = radial;
                kmVec2Scale(&radial, &radial, p->radaccel);
                //compute tangential
                cxFloat newy = tangential.x;
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
            // color
            p->color.r += (p->deltacolor.r * dt);
            p->color.g += (p->deltacolor.g * dt);
            p->color.b += (p->deltacolor.b * dt);
            p->color.a += (p->deltacolor.a * dt);
            //size
            p->size = CX_MAX(0, p->size + p->deltasize * dt);
            //angle
            p->rotation += (p->deltarotation * dt);
            cxParticleSetBox(this, p);
            this->index ++;
        }else{
            if(this->index != (this->count - 1)){
                this->units[this->index] = this->units[this->count - 1];
            }
            this->count--;
        }
    }
    if(!this->isActive && this->count <= 0){
        cxActionStop(pav);
    }
    cxAtlasSetNumber(this->atlas, this->count);
}

void cxParticleSetType(cxAny pview,cxParticleEmitterType type)
{
    cxParticle this = pview;
    this->type = type;
}

void cxParticleSetBlendMode(cxAny pav,cxParticleBlendMode mode)
{
    cxParticle this = pav;
    if(mode == cxParticleBlendAdd){
        cxSpriteSetBlendFactor(this->atlas, GL_SRC_ALPHA, GL_ONE);
    }else{
        cxSpriteSetBlendFactor(this->atlas, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    this->blend = mode;
}

static void cxParticleReset(cxAny pav)
{
    cxParticle this = pav;
    this->isActive = true;
    this->emitcounter = 0;
    for(this->index = 0; this->index < this->count; this->index++){
        cxParticleUnit *p = &this->units[this->index];
        p->life = 0;
    }
    cxAtlasClean(this->atlas);
}

CX_OBJECT_INIT(cxParticle, cxAction)
{
    this->atlas = CX_ALLOC(cxAtlas);
    this->rate = -1;
    this->isActive = true;
    this->type = cxParticleEmitterGravity;
    cxParticleSetBlendMode(this, cxParticleBlendAdd);
    CX_METHOD_SET(this->super.Reset, cxParticleReset);
    CX_METHOD_SET(this->super.Init, cxParticleInit);
    CX_METHOD_SET(this->super.Over, cxParticleOver);
    CX_METHOD_SET(this->super.Step, cxParticleStep);
}
CX_OBJECT_FREE(cxParticle, cxAction)
{
    allocator->free(this->units);
    CX_RELEASE(this->atlas);
    CX_SLOT_RELEASE(this->onDraw);
}
CX_OBJECT_TERM(cxParticle, cxAction)

void cxParticleInitNumber(cxAny pav,cxInt number)
{
    cxParticle this = pav;
    this->number = number;
    this->units = allocator->calloc(number,sizeof(cxParticleUnit));
    cxAtlasSetCapacity(this->atlas, number);
}

cxParticle cxParticleCreate(cxFloat duration,cxConstChars url,cxIndex number)
{
    cxParticle this = CX_CREATE(cxParticle);
    this->super.duration = -1;
    this->duration = duration;
    cxParticleInitNumber(this,number);
    cxSpriteSetTextureURL(this->atlas, url, true);
    cxSetRandSeed();
    return this;
}





