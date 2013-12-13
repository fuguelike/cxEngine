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

/*
 <particleEmitterConfig>
     <texture name="texture.png"/>
     <sourcePosition x="300.00" y="300.00"/>
     <sourcePositionVariance x="0.00" y="0.00"/>
     <speed value="100.00"/>
     <speedVariance value="30.00"/>
     <particleLifeSpan value="2.0000"/>
     <particleLifespanVariance value="1.9000"/>
     <angle value="270.00"/>
     <angleVariance value="2.00"/>
     <gravity x="0.00" y="0.00"/>
     <radialAcceleration value="0.00"/>
     <tangentialAcceleration value="0.00"/>
     <radialAccelVariance value="0.00"/>
     <tangentialAccelVariance value="0.00"/>
     <startColor red="1.00" green="0.31" blue="0.00" alpha="0.62"/>
     <startColorVariance red="0.00" green="0.00" blue="0.00" alpha="0.00"/>
     <finishColor red="1.00" green="0.31" blue="0.00" alpha="0.00"/>
     <finishColorVariance red="0.00" green="0.00" blue="0.00" alpha="0.00"/>
     <maxParticles value="500"/>
     <startParticleSize value="70.00"/>
     <startParticleSizeVariance value="49.53"/>
     <finishParticleSize value="10.00"/>
     <FinishParticleSizeVariance value="5.00"/>
     <particleToDir value="false" />
     <duration value="-1.00"/>
     <emitterType value="0"/>
     <emitterRate value="-1"/>
     <maxRadius value="100.00"/>
     <maxRadiusVariance value="0.00"/>
     <minRadius value="0.00"/>
     <rotatePerSecond value="0.00"/>
     <rotatePerSecondVariance value="0.00"/>
     <blendFuncSource value="770"/>
     <blendFuncDestination value="1"/>
     <rotationStart value="0.00"/>
     <rotationStartVariance value="0.00"/>
     <rotationEnd value="0.00"/>
     <rotationEndVariance value="0.00"/>
 </particleEmitterConfig>
 */

static void cxParticleXMLReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxParticle this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

void cxParticleInitFromPEX(cxAny pview,cxConstChars file)
{
    cxParticle this = pview;
    cxString data = cxAssertsData(file);
    CX_RETURN(data == NULL);
    xmlTextReaderPtr reader = cxXMLReaderForString(data,cxParticleXMLReaderError, this);
    while(xmlTextReaderRead(reader) && !this->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(ELEMENT_IS_TYPE(texture)){
            cxConstChars name = cxXMLAttr("name");
            cxSpriteSetTextureURL(this, name, true, true);
        }else if(ELEMENT_IS_TYPE(sourcePosition)){
            this->position.v.x = 0;//cxXMLReadFloatAttr(reader,NULL, "x", 0);
            this->position.v.y = 0;//cxXMLReadFloatAttr(reader,NULL, "y", 0);
        }else if(ELEMENT_IS_TYPE(sourcePositionVariance)){
            this->position.r.x = cxXMLReadFloatAttr(reader, NULL, "x", 0);
            this->position.r.y = cxXMLReadFloatAttr(reader, NULL,  "y", 0);
        }else if(ELEMENT_IS_TYPE(speed)){
            this->speed.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if(ELEMENT_IS_TYPE(speedVariance)){
            this->speed.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if(ELEMENT_IS_TYPE(particleLifeSpan)){
            this->life.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if(ELEMENT_IS_TYPE(particleLifespanVariance)){
            this->life.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if(ELEMENT_IS_TYPE(angle)){
            this->angle.v = -cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if(ELEMENT_IS_TYPE(angleVariance)){
            this->angle.r = -cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if(ELEMENT_IS_TYPE(gravity)){
            this->gravity.x = cxXMLReadFloatAttr(reader, NULL,  "x", 0);
            this->gravity.y = cxXMLReadFloatAttr(reader, NULL,  "y", 0);
        }else if (ELEMENT_IS_TYPE(radialAcceleration)){
            this->radaccel.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(radialAccelVariance)){
            this->radaccel.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(tangentialAcceleration)){
            this->tanaccel.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(tangentialAccelVariance)){
            this->tanaccel.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(startColor)){
            this->startcolor.v.r = cxXMLReadFloatAttr(reader, NULL,  "red", 0);
            this->startcolor.v.g = cxXMLReadFloatAttr(reader, NULL,  "green", 0);
            this->startcolor.v.b = cxXMLReadFloatAttr(reader, NULL,  "blue", 0);
            this->startcolor.v.a = cxXMLReadFloatAttr(reader, NULL,  "alpha", 0);
        }else if (ELEMENT_IS_TYPE(startColorVariance)){
            this->startcolor.r.r = cxXMLReadFloatAttr(reader, NULL,  "red", 0);
            this->startcolor.r.g = cxXMLReadFloatAttr(reader, NULL,  "green", 0);
            this->startcolor.r.b = cxXMLReadFloatAttr(reader, NULL,  "blue", 0);
            this->startcolor.r.a = cxXMLReadFloatAttr(reader, NULL,  "alpha", 0);
        }else if (ELEMENT_IS_TYPE(finishColor)){
            this->endcolor.v.r = cxXMLReadFloatAttr(reader, NULL,  "red", 0);
            this->endcolor.v.g = cxXMLReadFloatAttr(reader, NULL,  "green", 0);
            this->endcolor.v.b = cxXMLReadFloatAttr(reader, NULL,  "blue", 0);
            this->endcolor.v.a = cxXMLReadFloatAttr(reader, NULL,  "alpha", 0);
        }else if (ELEMENT_IS_TYPE(finishColorVariance)){
            this->endcolor.r.r = cxXMLReadFloatAttr(reader, NULL,  "red", 0);
            this->endcolor.r.g = cxXMLReadFloatAttr(reader, NULL,  "green", 0);
            this->endcolor.r.b = cxXMLReadFloatAttr(reader, NULL,  "blue", 0);
            this->endcolor.r.a = cxXMLReadFloatAttr(reader, NULL,  "alpha", 0);
        }else if (ELEMENT_IS_TYPE(maxParticles)){
            cxInt number = cxXMLReadIntAttr(reader, NULL, "value", 0);
            CX_ASSERT(number > 0, "particle number must > 0");
            cxParticleInit(this, number);
        }else if (ELEMENT_IS_TYPE(startParticleSize)){
            this->startsize.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(startParticleSizeVariance)){
            this->startsize.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(finishParticleSize)){
            this->endsize.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(FinishParticleSizeVariance)){
            this->endsize.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(duration)){
            this->duration = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(emitterType)){
            this->type = cxXMLReadIntAttr(reader, NULL, "value", cxParticleEmitterGravity);
        }else if (ELEMENT_IS_TYPE(blendFuncSource)){
            this->super.super.sfactor = cxXMLReadIntAttr(reader, NULL, "value", GL_SRC_ALPHA);
        }else if (ELEMENT_IS_TYPE(blendFuncDestination)){
            this->super.super.dfactor = cxXMLReadIntAttr(reader, NULL, "value", GL_ONE_MINUS_SRC_ALPHA);
        }else if (ELEMENT_IS_TYPE(emitterRate)){
            this->rate = cxXMLReadFloatAttr(reader, NULL,  "value", this->rate);
        }else if (ELEMENT_IS_TYPE(particleToDir)){
            this->todir = cxXMLReadBoolAttr(reader,NULL, "value", this->todir);
        }else if (ELEMENT_IS_TYPE(rotatePerSecond)){
            this->rotatepers.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if(ELEMENT_IS_TYPE(rotatePerSecondVariance)){
            this->rotatepers.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(rotationStart)){
            this->startspin.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(rotationStartVariance)){
            this->startspin.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(rotationEnd)){
            this->endspin.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(rotationEndVariance)){
            this->endspin.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(maxRadius)){
            this->startradius.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(maxRadiusVariance)){
            this->startradius.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(minRadius)){
            this->endradius.v = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }else if (ELEMENT_IS_TYPE(minRadiusVariance)){
            this->endradius.r = cxXMLReadFloatAttr(reader, NULL,  "value", 0);
        }
    }
}

cxNumber cxpEmitterType(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "arg error");
    cxConstChars mode = cxEventArgToString(arg);
    if(cxConstCharsEqu(mode, "gravity")){
        return cxNumberInt(cxParticleEmitterGravity);
    }
    if(cxConstCharsEqu(mode, "radial")){
        return cxNumberInt(cxParticleEmitterRadial);
    }
    CX_ERROR("Emtter type gravity or radial");
    return NULL;
}

cxNumber cxpBlendMode(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "arg error");
    cxConstChars mode = cxEventArgToString(arg);
    if(cxConstCharsEqu(mode, "add")){
        return cxNumberInt(cxParticleBlendAdd);
    }
    if(cxConstCharsEqu(mode, "multiple")){
        return cxNumberInt(cxParticleBlendAdd);
    }
    CX_ERROR("blend mode add or multiple");
    return NULL;
}

void cxParticleReadAttr(cxAny rootView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewRoot root = rootView;
    cxAtlasReadAttr(rootView, mView, reader);
    cxParticle this = mView;
    cxConstChars pex = cxXMLAttr("cxParticle.pex");
    if(pex != NULL){
        cxParticleInitFromPEX(mView, pex);
        return;
    }
    cxInt number = cxXMLReadIntAttr(reader, root->functions, "cxParticle.number", this->number);
    CX_ASSERT(number > 0, "cxParticle number must > 0");
    cxParticleInit(this, number);

    cxParticleSetBlendMode(mView, cxXMLReadIntAttr(reader, root->functions, "cxParticle.blend", cxParticleBlendMultiply));
    
    cxParticleSetType(mView, cxXMLReadIntAttr(reader, root->functions, "cxParticle.type", cxParticleEmitterGravity));

    this->todir         = cxXMLReadBoolAttr(reader,root->functions, "cxParticle.todir", this->todir);
    this->duration      = cxXMLReadFloatAttr(reader, root->functions, "cxParticle.duration", this->duration);
    this->gravity       = cxXMLReadVec2fAttr(reader, root->functions, "cxParticle.gravity", this->gravity);
    this->rate          = cxXMLReadFloatAttr(reader, root->functions, "cxParticle.rate", this->rate);
    this->speed         = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.speed", this->speed);
    this->tanaccel      = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.tanaccel", this->tanaccel);
    this->radaccel      = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.radaccel", this->radaccel);
    this->position      = cxXMLReadVec2fRangeAttr(reader, root->functions, "cxParticle.position", this->position);
    this->life          = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.life", this->life);
    this->angle         = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.angle", this->angle);
    this->startsize     = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.startsize", this->startsize);
    this->endsize       = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.endsize", this->endsize);
    this->startcolor    = cxXMLReadColor4fRangeAttr(reader, root->functions, "cxParticle.startcolor", this->startcolor);
    this->endcolor      = cxXMLReadColor4fRangeAttr(reader, root->functions, "cxParticle.endcolor", this->endcolor);
    this->startspin     = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.startspin", this->startspin);
    this->endspin       = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.endspin", this->endspin);
    this->startradius   = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.startradius", this->startradius);
    this->endradius     = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.endradius", this->endradius);
    this->rotatepers    = cxXMLReadFloatRangeAttr(reader, root->functions, "cxParticle.rotatepers", this->rotatepers);
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

static void cxParticleUpdate(cxEvent *event)
{
    cxEngine engine = cxEngineInstance();
    cxFloat dt = engine->frameDelta;
    cxParticle this = event->sender;
    if(this->isActive){
        cxFloat rate = 1.0f / this->rate;
        if(this->count < this->number){
            this->emitcounter += dt;
        }
        while(this->count < this->number && this->emitcounter > rate && cxParticleAdd(this)){
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
    cxViewOnUpdate(this, cxParticleUpdate);
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE);
    cxObjectSetReadAttrFunc(this, cxParticleReadAttr);
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




