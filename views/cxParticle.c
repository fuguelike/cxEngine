//
//  cxParticle.c
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxParticle.h"

void cxParticleFireInitUnit(cxAny pivew,cxParticleUnit *particle,cxInt index)
{
    CX_ASSERT_THIS(pivew, cxParticle);
    cxParticleUnitFixInfo info={0};
    info.speed = cxFloatRandValue(this->speed);
    info.angle = kmDegreesToRadians(cxFloatRandValue(this->angle));
    info.position = cxVec2fRandValue(this->position);
    info.life = cxFloatRandValue(this->life);
    info.unit = particle;
    
    CX_CALL(this, FixUnit, CX_M(void,cxParticleUnitFixInfo *),&info);
    
    particle->position = info.position;
    particle->life = info.life;
    cxColor4f scolor = cxColor4fRandValue(this->startcolor);
    cxColor4f ecolor = cxColor4fRandValue(this->endcolor);
    cxFloat ssize = cxFloatRandValue(this->startsize);
    cxFloat esize = cxFloatRandValue(this->endsize);
    cxFloat sspin = cxFloatRandValue(this->startspin);
    cxFloat espin = cxFloatRandValue(this->endspin);
    
    particle->color = scolor;
    particle->deltacolor.r = (ecolor.r - scolor.r) / particle->life;
    particle->deltacolor.g = (ecolor.g - scolor.g) / particle->life;
    particle->deltacolor.b = (ecolor.b - scolor.b) / particle->life;
    particle->deltacolor.a = (ecolor.a - scolor.a) / particle->life;
    particle->size = ssize;
    particle->deltasize = (esize - ssize) / particle->life;
    particle->rotation = sspin;
    particle->deltarotation = (espin - sspin) / particle->life;
    
    if(this->type == cxParticleEmitterGravity){
        cxVec2f v = cxVec2fv(cosf(info.angle), sinf(info.angle));
        particle->dir = cxVec2fScale(v, info.speed);
        particle->radaccel = cxFloatRandValue(this->radaccel);
        particle->tanaccel = cxFloatRandValue(this->tanaccel);
        if(this->todir){
            particle->rotation = -kmRadiansToDegrees(cxVec2fAngle(particle->dir));
        }
    }else{
        cxFloat startradius = cxFloatRandValue(this->startradius);
        cxFloat endradius = cxFloatRandValue(this->endradius);
        particle->radius = startradius;
        if(endradius == -1){
            particle->deltaradius = 0;
        }else{
            particle->deltaradius = (endradius - startradius) / particle->life;
        }
        particle->angle = info.angle;
        particle->degreespers = kmDegreesToRadians(cxFloatRandValue(this->rotatepers));
    }
}

static cxBool cxParticleAdd(cxParticle this)
{
    if(this->count >= this->number){
        return false;
    }
    cxParticleUnit *unit = &this->units[this->count];
    cxParticleFireInitUnit(this, unit, this->count);
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
    CX_ASSERT_THIS(pav, cxParticle);
    cxBoxVec3f vq={0};
    cxColor4f color = particle->color;
    cxParticleUnitToBoxVec3f(particle, &vq);
    cxBoxPoint *box = cxAtlasBoxPoint(this, this->index);
    box->lb.colors = color;
    box->rb.colors = color;
    box->lt.colors = color;
    box->rt.colors = color;
    cxBoxTex2f *texCoord = cxSpriteGetTexCoord(this);
    box->lb.texcoords = texCoord->lb;
    box->rb.texcoords = texCoord->rb;
    box->lt.texcoords = texCoord->lt;
    box->rt.texcoords = texCoord->rt;
    box->lb.vertices = vq.lb;
    box->rb.vertices = vq.rb;
    box->lt.vertices = vq.lt;
    box->rt.vertices = vq.rt;
    CX_CALL(this, UpdateUnit, CX_M(void,cxParticleUnit *),particle);
}

void cxParticlePlay(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxParticle);
    this->isActive = true;
    this->emitcounter = 0;
    for(this->index = 0; this->index < this->count; this->index++){
        cxParticleUnit *p = &this->units[this->index];
        p->life = 0;
    }
    this->count = 0;
    cxAtlasClear(this);
}

void cxParticleStop(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxParticle);
    this->isActive = false;
    this->emitcounter = 0;
    this->isInit = false;
}

static void cxParticleUpdate(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxParticle);
    if(this->number == 0){
        return;
    }
    if(!this->isInit){
        this->isInit = true;
        CX_CALL(this, Init, CX_M(void));
    }
    cxFloat dt = cxEngineGetDelta();
    this->TimeElapsed += dt;
    if(this->isActive){
        cxFloat rate = 1.0f / this->rate;
        if(this->count < this->number){
            this->emitcounter += dt;
        }
        while(this->count < this->number && this->emitcounter > rate){
            cxParticleAdd(this);
            this->emitcounter -= rate;
        }
		if (this->time != -1 && this->time < this->TimeElapsed){
			cxParticleStop(pview);
        }
    }
    this->index = 0;
    while(this->index < this->count){
        cxParticleUnit *p = &this->units[this->index];
        p->life -= dt;
        if(p->life <= 0){
            if(this->index != (this->count - 1)){
                this->units[this->index] = this->units[this->count - 1];
            }
            this->count--;
            continue;
        }
        //vertex
        if(this->type == cxParticleEmitterGravity){
            cxVec2f tmp;
            cxVec2f radial= cxVec2fv(0, 0);
            if(p->position.x || p->position.y){
                kmVec2Normalize(&radial, &p->position);
            }
            cxVec2f tangential = radial;
            radial = cxVec2fScale(radial, p->radaccel);
            //compute tangential
            cxFloat newy = tangential.x;
            tangential.x = -tangential.y;
            tangential.y = newy;
            tangential = cxVec2fScale(tangential, p->tanaccel);
            //compute position
            tmp = cxVec2fAdd(radial, tangential);
            tmp = cxVec2fAdd(tmp, this->gravity);
            tmp = cxVec2fScale(tmp, dt);
            p->dir = cxVec2fAdd(p->dir, tmp);
            tmp = cxVec2fScale(p->dir, dt);
            p->position = cxVec2fAdd(p->position, tmp);
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
    }
    if(!this->isActive && this->count <= 0){
        cxParticleStop(pview);
    }
    cxAtlasSetNumber(this, this->count);
}

void cxParticleSetType(cxAny pav,cxParticleEmitterType type)
{
    CX_ASSERT_THIS(pav, cxParticle);
    this->type = type;
}

CX_SETTER_DEF(cxParticle, number)
{
    cxInt number = cxJsonToInt(value, this->number);
    cxParticleInitNumber(this, number);
}

CX_SETTER_DEF(cxParticle, emitter)
{
    cxConstChars type = cxJsonToConstChars(value);
    if(cxConstCharsEqu(type, "gravity")){
        cxParticleSetType(this, cxParticleEmitterGravity);
    }else if(cxConstCharsEqu(type, "radial")){
        cxParticleSetType(this, cxParticleEmitterRadial);
    }else{
        cxParticleSetType(this, cxParticleEmitterGravity);
    }
}
CX_SETTER_DEF(cxParticle, time)
{
    this->time = cxJsonToDouble(value, this->time);
}
CX_SETTER_DEF(cxParticle, life)
{
    this->life = cxJsonToFloatRangle(value,this->life);
}
CX_SETTER_DEF(cxParticle, position)
{
    this->position = cxJsonToVec2fRangle(value,this->position);
}
CX_SETTER_DEF(cxParticle, rate)
{
    this->rate = cxJsonToDouble(value, this->rate);
}
CX_SETTER_DEF(cxParticle, angle)
{
    this->angle = cxJsonToFloatRangle(value,this->angle);
}
CX_SETTER_DEF(cxParticle, ssize)
{
    this->startsize = cxJsonToFloatRangle(value,this->startsize);
}
CX_SETTER_DEF(cxParticle, esize)
{
    this->endsize = cxJsonToFloatRangle(value, this->endsize);
}
CX_SETTER_DEF(cxParticle, scolor)
{
    this->startcolor = cxJsonToColor4fRangle(value,this->startcolor);
}
CX_SETTER_DEF(cxParticle, ecolor)
{
    this->endcolor = cxJsonToColor4fRangle(value,this->endcolor);
}
CX_SETTER_DEF(cxParticle, sspin)
{
    this->startspin = cxJsonToFloatRangle(value,this->startspin);
}
CX_SETTER_DEF(cxParticle, espin)
{
    this->endspin = cxJsonToFloatRangle(value,this->endspin);
}
CX_SETTER_DEF(cxParticle, gravity)
{
    this->gravity = cxJsonToVec2f(value,this->gravity);
}
CX_SETTER_DEF(cxParticle, todir)
{
    this->todir = cxJsonToBool(value, this->todir);
}
CX_SETTER_DEF(cxParticle, tanaccel)
{
    this->tanaccel = cxJsonToFloatRangle(value,this->tanaccel);
}
CX_SETTER_DEF(cxParticle, radaccel)
{
    this->radaccel = cxJsonToFloatRangle(value,this->radaccel);
}
CX_SETTER_DEF(cxParticle, speed)
{
    this->speed = cxJsonToFloatRangle(value,this->speed);
}
CX_SETTER_DEF(cxParticle, sradius)
{
    this->startradius = cxJsonToFloatRangle(value,this->startradius);
}
CX_SETTER_DEF(cxParticle, eradius)
{
    this->endradius = cxJsonToFloatRangle(value,this->endradius);
}
CX_SETTER_DEF(cxParticle, rotatepers)
{
    this->rotatepers = cxJsonToFloatRangle(value,this->rotatepers);
}

CX_METHOD_DEF(cxParticle, FixUnit, void, cxParticleUnitFixInfo *info)
{
    cxAny v = cxViewBindesFirst(this);
    CX_RETURN(v == NULL);
    info->angle = cxViewGetMoveAngle(v) + M_PI;
    info->position = cxViewGetPosition(v);
}
CX_METHOD_DEF(cxParticle, UpdateUnit, void,cxParticleUnit *unit)
{
    
}
CX_METHOD_DEF(cxParticle, Init, void)
{
    if(this->rate < 0){
        this->rate = (this->life.v + this->life.r) * this->number;
    }
}
CX_TYPE(cxParticle, cxAtlas)
{
    CX_SETTER(cxParticle, number);
    CX_SETTER(cxParticle, emitter);
    CX_SETTER(cxParticle, time);
    CX_SETTER(cxParticle, life);
    CX_SETTER(cxParticle, position);
    CX_SETTER(cxParticle, rate);
    CX_SETTER(cxParticle, angle);
    CX_SETTER(cxParticle, ssize);      //start size
    CX_SETTER(cxParticle, esize);      //end size
    CX_SETTER(cxParticle, scolor);     //start color
    CX_SETTER(cxParticle, ecolor);     //end color
    CX_SETTER(cxParticle, sspin);      //start spin
    CX_SETTER(cxParticle, espin);      //end spin
    CX_SETTER(cxParticle, gravity);
    CX_SETTER(cxParticle, todir);
    CX_SETTER(cxParticle, tanaccel);
    CX_SETTER(cxParticle, radaccel);
    CX_SETTER(cxParticle, speed);
    CX_SETTER(cxParticle, sradius);     //start radius
    CX_SETTER(cxParticle, eradius);     //end radius
    CX_SETTER(cxParticle, rotatepers);  // rotate/s
    
    CX_METHOD(cxParticle, UpdateUnit);
    CX_METHOD(cxParticle, FixUnit);
    CX_METHOD(cxParticle, Init);
}
CX_INIT(cxParticle, cxAtlas)
{
    cxSetRandSeed();
    this->rate = CX_FOREVER;
    this->time = CX_FOREVER;
    this->isActive = true;
    this->type = cxParticleEmitterGravity;
    CX_ADD(cxView, this, onUpdate, cxParticleUpdate);
}
CX_FREE(cxParticle, cxAtlas)
{
    allocator->free(this->units);
}
CX_TERM(cxParticle, cxAtlas)

void cxParticleInitNumber(cxAny pav,cxInt number)
{
    CX_ASSERT_THIS(pav, cxParticle);
    CX_RETURN(this->number >= number);
    this->number = number;
    this->units = allocator->realloc(this->units,sizeof(cxParticleUnit)*number);
    cxAtlasSetCapacity(this, number);
}

cxParticle cxParticleCreate(cxFloat time,cxConstChars url,cxInt number)
{
    cxParticle this = CX_CREATE(cxParticle);
    this->time = time;
    cxParticleInitNumber(this,number);
    cxSpriteSetTextureURL(this, url);
    return this;
}





