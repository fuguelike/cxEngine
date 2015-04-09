//
//  cxParticle.c
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxParticle.h"

void cxParticleFireInitUnit(cxAny pav,cxParticleUnit *particle,cxInt index)
{
    CX_ASSERT_THIS(pav, cxParticle);
    
    cxParticleUnitInfo info={0};
    info.speed = cxFloatRandValue(this->speed);
    info.angle = kmDegreesToRadians(cxFloatRandValue(this->angle));
    info.position = cxVec2fRandValue(this->position);
    info.life = cxFloatRandValue(this->life);
    info.unit = particle;
    
    CX_CALL(this, FixUnit, CX_M(void,cxParticleUnitInfo *),&info);
    
    particle->position = info.position;
    particle->life = info.life;
    cxColor4f scolor = cxColor4fRandValue(this->startcolor);
    cxColor4f ecolor = cxColor4fRandValue(this->endcolor);
    cxFloat ssize = cxFloatRandValue(this->startsize);
    cxFloat esize = cxFloatRandValue(this->endsize);
    cxFloat sspin = kmDegreesToRadians(cxFloatRandValue(this->startspin));
    cxFloat espin = kmDegreesToRadians(cxFloatRandValue(this->endspin));

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
        particle->dir = cxVec2fScale1f(v, info.speed);
        particle->radaccel = cxFloatRandValue(this->radaccel);
        particle->tanaccel = cxFloatRandValue(this->tanaccel);
        if(this->todir){
            particle->rotation = -cxVec2fAngle(particle->dir);
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

CX_INLINE cxBool cxParticleAdd(cxParticle this)
{
    if(this->count >= this->number){
        return false;
    }
    cxParticleUnit *unit = &this->units[this->count];
    cxParticleFireInitUnit(this, unit, this->count);
    this->count ++;
    return true;
}

CX_INLINE cxVec3f cxParticleTransform(const kmMat4 *mat4,cxVec3f v,cxFloat x,cxFloat y)
{
    cxVec3f r;
    kmVec3MultiplyMat4(&r, &v, mat4);
    return cxVec3fv(r.x + x, r.y + y, r.z);
}
CX_INLINE void cxParticleUnitToBoxVec3f(cxParticle this,cxParticleUnit *particle,cxBoxVec3f *vq)
{
    cxFloat sizeh = particle->size/2;
    cxFloat x = particle->position.x;
    cxFloat y = particle->position.y;
    cxFloat l = -sizeh;
    cxFloat b = -sizeh;
    cxFloat r = +sizeh;
    cxFloat t = +sizeh;
    if(this->todir){
        cxVec3f axis =  cxVec3fv(particle->dir.x, particle->dir.y, 0);
        kmVec3Normalize(&this->axisspin, &axis);
    }
    if (particle->rotation){
        kmMat4 mat4;
        kmMat4RotationAxisAngle(&mat4, &this->axisspin, -particle->rotation);
        vq->lb = cxParticleTransform(&mat4, cxVec3fv(l, b, 0), x, y);
        vq->rb = cxParticleTransform(&mat4, cxVec3fv(r, b, 0), x, y);
        vq->rt = cxParticleTransform(&mat4, cxVec3fv(r, t, 0), x, y);
        vq->lt = cxParticleTransform(&mat4, cxVec3fv(l, t, 0), x, y);
    }else{
        vq->lb.x = x + l;
        vq->lb.y = y + b;
        vq->rb.x = x + r;
        vq->rb.y = y + b;
        vq->lt.x = x + l;
        vq->lt.y = y + t;
        vq->rt.x = x + r;
        vq->rt.y = y + t;
    }
}

CX_INLINE void cxParticleSetBox(cxAny pav,cxParticleUnit *particle)
{
    CX_ASSERT_THIS(pav, cxParticle);
    CX_ASSERT_VALUE(cxActionGetView(this), cxAtlas, atlas);
    cxBoxVec3f vq={0};
    cxColor4f color = particle->color;
    cxParticleUnitToBoxVec3f(this,particle, &vq);
    cxBoxPoint *box = cxAtlasBoxPoint(atlas, this->index + this->idxOffset);
    box->lb.colors = color;
    box->rb.colors = color;
    box->lt.colors = color;
    box->rt.colors = color;
    cxBoxTex2f *texCoord = CX_CALL(this, GetTexCoord, CX_M(cxBoxTex2f *,cxParticleUnit *),particle);
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

void cxParticleSetType(cxAny pav,cxParticleEmitterType type)
{
    CX_ASSERT_THIS(pav, cxParticle);
    this->type = type;
}
CX_SETTER_DEF(cxParticle, offset)
{
    cxParticleSetOffset(this, cxJsonToInt(value, this->idxOffset));
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
CX_SETTER_DEF(cxParticle, life)
{
    this->life = cxJsonToFloatRange(value,this->life);
}
CX_SETTER_DEF(cxParticle, position)
{
    this->position = cxJsonToVec2fRange(value,this->position);
}
CX_SETTER_DEF(cxParticle, rate)
{
    this->rate = cxJsonToDouble(value, this->rate);
}
CX_SETTER_DEF(cxParticle, angle)
{
    this->angle = cxJsonToFloatRange(value,this->angle);
}
CX_SETTER_DEF(cxParticle, ssize)
{
    this->startsize = cxJsonToFloatRange(value,this->startsize);
}
CX_SETTER_DEF(cxParticle, esize)
{
    this->endsize = cxJsonToFloatRange(value, this->endsize);
}
CX_SETTER_DEF(cxParticle, scolor)
{
    this->startcolor = cxJsonToColor4fRange(value,this->startcolor);
}
CX_SETTER_DEF(cxParticle, ecolor)
{
    this->endcolor = cxJsonToColor4fRange(value,this->endcolor);
}
CX_SETTER_DEF(cxParticle, sspin)
{
    this->startspin = cxJsonToFloatRange(value,this->startspin);
}
CX_SETTER_DEF(cxParticle, espin)
{
    this->endspin = cxJsonToFloatRange(value,this->endspin);
}
CX_SETTER_DEF(cxParticle, aspin)
{
    this->axisspin = cxJsonToVec3f(value, this->axisspin);
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
    this->tanaccel = cxJsonToFloatRange(value,this->tanaccel);
}
CX_SETTER_DEF(cxParticle, radaccel)
{
    this->radaccel = cxJsonToFloatRange(value,this->radaccel);
}
CX_SETTER_DEF(cxParticle, speed)
{
    this->speed = cxJsonToFloatRange(value,this->speed);
}
CX_SETTER_DEF(cxParticle, sradius)
{
    this->startradius = cxJsonToFloatRange(value,this->startradius);
}
CX_SETTER_DEF(cxParticle, eradius)
{
    this->endradius = cxJsonToFloatRange(value,this->endradius);
}
CX_SETTER_DEF(cxParticle, rotatepers)
{
    this->rotatepers = cxJsonToFloatRange(value,this->rotatepers);
}
CX_METHOD_DEF(cxParticle, FixUnit, void, cxParticleUnitInfo *info)
{
    cxAny view = cxActionGetView(this);
    CX_RETURN(view == NULL);
    cxAny v = cxViewBindesFirst(view);
    CX_RETURN(v == NULL);
//    info->angle = cxViewGetMoveAngle(v) + M_PI;
    info->position = cxViewGetPosition(v);
}
CX_METHOD_DEF(cxParticle, UpdateUnit, void,cxParticleUnit *unit)
{
    
}
CX_METHOD_DEF(cxParticle, Init, void)
{
    //must is cxAtlas view
    CX_ASSERT_VALUE(cxActionGetView(this), cxAtlas, atlas);
    if(this->rate < 0){
        this->rate = (this->life.v + this->life.r) * this->number * fabsf(this->rate);
    }
    cxAtlasSetCapacity(atlas, this->number + this->idxOffset);
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxParticle,Step,void,cxFloat dt, cxFloat time)
{
    if(this->number == 0){
        return;
    }
    if(this->isActive){
        cxFloat rate = 1.0f / this->rate;
        if(this->count < this->number){
            this->emitcounter += dt;
        }
        while(this->count < this->number && this->emitcounter > rate){
            cxParticleAdd(this);
            this->emitcounter -= rate;
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
            radial = cxVec2fScale1f(radial, p->radaccel);
            //compute tangential
            cxFloat newy = tangential.x;
            tangential.x = -tangential.y;
            tangential.y = newy;
            tangential = cxVec2fScale1f(tangential, p->tanaccel);
            //compute position
            tmp = cxVec2fAdd(radial, tangential);
            tmp = cxVec2fAdd(tmp, this->gravity);
            tmp = cxVec2fScale1f(tmp, dt);
            p->dir = cxVec2fAdd(p->dir, tmp);
            tmp = cxVec2fScale1f(p->dir, dt);
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
    CX_ASSERT_VALUE(cxActionGetView(this), cxAtlas, atlas);
    cxAtlasSetNumber(atlas, this->count);
    cxAtlasSetDirty(atlas);
    //
    if(!this->isActive && this->count <= 0){
        cxActionStop(this);
    }
}
CX_METHOD_DEF(cxParticle, Reset, void)
{
    this->isActive = true;
    this->emitcounter = 0;
    for(this->index = 0; this->index < this->count; this->index++){
        cxParticleUnit *p = &this->units[this->index];
        p->life = 0;
    }
    this->count = 0;
    CX_SUPER(cxAction, this, Reset, CX_M(void));
}
CX_METHOD_DEF(cxParticle,IsExit,cxBool)
{
    this->isActive = false;
    return false;
}
CX_METHOD_DEF(cxParticle, GetTexCoord, cxBoxTex2f *,cxParticleUnit *unit)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxAtlas, atlas);
    return cxSpriteGetTexCoord(atlas);
}
CX_TYPE(cxParticle, cxAction)
{
    CX_SETTER(cxParticle, number);
    CX_SETTER(cxParticle, offset);  //atlas index offset
    CX_SETTER(cxParticle, emitter);
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
    CX_SETTER(cxParticle, aspin);      //roration axis
    CX_SETTER(cxParticle, gravity);
    CX_SETTER(cxParticle, todir);
    CX_SETTER(cxParticle, tanaccel);
    CX_SETTER(cxParticle, radaccel);
    CX_SETTER(cxParticle, speed);
    CX_SETTER(cxParticle, sradius);     //start radius
    CX_SETTER(cxParticle, eradius);     //end radius
    CX_SETTER(cxParticle, rotatepers);  // rotate/s
    
    CX_METHOD(cxParticle, GetTexCoord);
    CX_METHOD(cxParticle, UpdateUnit);
    CX_METHOD(cxParticle, FixUnit);
    CX_METHOD(cxParticle, Init);
    CX_METHOD(cxParticle, Step);
    CX_METHOD(cxParticle, Reset);
    CX_METHOD(cxParticle, IsExit);
}
CX_INIT(cxParticle, cxAction)
{
    cxSetRandSeed();
    this->rate = CX_FOREVER;
    this->isActive = true;
    this->type = cxParticleEmitterGravity;
    this->axisspin = cxVec3fZ;
    cxActionSetTime(this, CX_FOREVER);
}
CX_FREE(cxParticle, cxAction)
{
    allocator->free(this->units);
}
CX_TERM(cxParticle, cxAction)

void cxParticleSetOffset(cxAny pav,cxInt offset)
{
    CX_ASSERT_THIS(pav, cxParticle);
    this->idxOffset = offset;
}

void cxParticleInitNumber(cxAny pav,cxInt number)
{
    CX_ASSERT_THIS(pav, cxParticle);
    if(number > this->number){
        this->units = allocator->realloc(this->units,sizeof(cxParticleUnit)*number);
    }
    this->number = number;
}





