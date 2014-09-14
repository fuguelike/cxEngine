//
//  cxSpine.c
//  cxEngine
//
//  Created by xuhua on 9/13/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include <engine/cxEngine.h>
#include "cxSpine.h"

//for spine
void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
    cxTexture texture = cxTextureFactoryLoadFile(path);
    CX_ASSERT(texture != NULL, "create texture failed %s",path);
    CX_RETAIN(texture);
    self->rendererObject = texture;
    self->width = texture->size.w;
    self->height = texture->size.h;
}
void _spAtlasPage_disposeTexture (spAtlasPage* self)
{
    cxTexture texture = self->rendererObject;
    CX_RELEASE(texture);
}

char * _spUtil_readFile (const char* path, int* length)
{
    CX_ASSERT_FALSE("don't use withfile function read file %s",path);
    return NULL;
}

void cxSpineUpdateBox(cxSpine this,spRegionAttachment *self,spSlot *slot)
{
    cxColor4f color = cxViewColor(this);
    this->skeleton->r = color.r;
	this->skeleton->g = color.g;
	this->skeleton->b = color.b;
	this->skeleton->a = color.a;
    
    cxBoxPoint bp;
    spRegionAttachment_computeWorldVertices(self, slot->bone, this->vertices);
    
    color.a = this->skeleton->a * self->a * slot->a;
    color.r = this->skeleton->r * self->r * slot->r;
    color.g = this->skeleton->g * self->g * slot->g;
    color.b = this->skeleton->b * self->b * slot->b;
    
    bp.lb.colors = color;
    bp.rb.colors = color;
    bp.lt.colors = color;
    bp.rt.colors = color;
    
    bp.lb.vertices = cxVec3fv(this->vertices[SP_VERTEX_X1], this->vertices[SP_VERTEX_Y1], 0);
    bp.lt.vertices = cxVec3fv(this->vertices[SP_VERTEX_X2], this->vertices[SP_VERTEX_Y2], 0);
    bp.rt.vertices = cxVec3fv(this->vertices[SP_VERTEX_X3], this->vertices[SP_VERTEX_Y3], 0);
    bp.rb.vertices = cxVec3fv(this->vertices[SP_VERTEX_X4], this->vertices[SP_VERTEX_Y4], 0);
    
    bp.lb.texcoords = cxTex2fv(self->uvs[SP_VERTEX_X1], self->uvs[SP_VERTEX_Y1]);
    bp.lt.texcoords = cxTex2fv(self->uvs[SP_VERTEX_X2], self->uvs[SP_VERTEX_Y2]);
    bp.rt.texcoords = cxTex2fv(self->uvs[SP_VERTEX_X3], self->uvs[SP_VERTEX_Y3]);
    bp.rb.texcoords = cxTex2fv(self->uvs[SP_VERTEX_X4], self->uvs[SP_VERTEX_Y4]);
    
    cxAtlasAppend(this, &bp);
}


static void cxSpineUpdate(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSpine);
    cxEngine engine = cxEngineInstance();
    spSkeleton_update(this->skeleton, engine->frameDelta);
	spSkeleton_updateWorldTransform(this->skeleton);
}

static void cxSpineDraw(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSpine);
    cxInt additive = 0;
	for (cxInt i = 0; i < this->skeleton->slotsCount; i++) {
		spSlot* slot = this->skeleton->drawOrder[i];
        if(slot->attachment == NULL || slot->attachment->type != SP_ATTACHMENT_REGION){
            continue;
        }
        spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
        cxTexture texture = ((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
        if(texture == NULL){
            continue;
        }
        if(slot->data->additiveBlending != additive){
            cxAtlasDraw(this);
            cxAtlasClean(this);
            additive = !additive;
            GLenum dfactor = additive ? GL_ONE : this->cxAtlas.cxSprite.dfactor;
            GLenum sfactor = this->cxAtlas.cxSprite.sfactor;
            cxSpriteSetBlendFactor(this, sfactor, dfactor);
        }else if(this->cxAtlas.cxSprite.texture != texture){
            cxSpriteSetTexture(this, texture);
            cxAtlasDraw(this);
            cxAtlasClean(this);
        }
        cxSpineUpdateBox(this, attachment, slot);
	}
    cxAtlasDraw(this);
    cxAtlasClean(this);
}
/*
"data":
 {
     "atlas":"atlas",
     "json":"json",
     "mix": [
        {
            "from": "walk",
            "to": "jump",
            "time": 0.2
        },
        {
            "from": "jump",
            "to": "run",
            "time": 0.4
        }
     ]
 }
 */

CX_SETTER_DEF(cxSpine, spine)
{
    cxConstChars atlas = cxJsonConstChars(value, "atlas");
    cxConstChars json = cxJsonConstChars(value, "json");
    cxJson mixs = cxJsonArray(value, "mix");
    cxFloat scale = cxJsonDouble(value, "scale", 1.0f);
    if(atlas != NULL && json != NULL){
        cxSpineInit(this, atlas, json, mixs, scale);
    }
}

CX_OBJECT_TYPE(cxSpine, cxAtlas)
{
    CX_PROPERTY_SETTER(cxSpine, spine);
}
CX_OBJECT_INIT(cxSpine, cxAtlas)
{
    CX_METHOD_SET(CX_TYPE(cxView, this)->Draw, cxSpineDraw);
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onUpdate, cxSpineUpdate);
    cxAtlasSetCapacity(this, 256);
}
CX_OBJECT_FREE(cxSpine, cxAtlas)
{
    if(this->stateData != NULL){
        spAnimationStateData_dispose(this->stateData);
    }
    if(this->skeletonData != NULL){
        spSkeletonData_dispose(this->skeletonData);
    }
    if(this->atlas != NULL){
        spAtlas_dispose(this->atlas);
    }
    if(this->skeleton != NULL){
        spSkeleton_dispose(this->skeleton);
    }
}
CX_OBJECT_TERM(cxSpine, cxAtlas)

void cxSpineSetMixs(cxAny pview,cxJson mixs)
{
    CX_ASSERT_THIS(pview, cxSpine);
    CX_ASSERT(mixs != NULL && cxJsonIsArray(mixs), "mixs args error");
    CX_JSON_ARRAY_EACH_BEG(mixs, mix)
    {
        cxConstChars from = cxJsonConstChars(mix, "from");
        cxConstChars to = cxJsonConstChars(mix, "to");
        cxFloat time = cxJsonDouble(mix, "time", 0);
        if(from != NULL && to != NULL){
            spAnimationStateData_setMixByName(this->stateData, from, to, time);
        }
    }
    CX_JSON_ARRAY_EACH_END(mixs, mix)
}

cxBool cxSpineInit(cxAny pview, cxConstChars atlas,cxConstChars json,cxJson mixs,cxFloat scale)
{
    CX_ASSERT(scale != 0, "scale zero");
    CX_ASSERT_THIS(pview, cxSpine);
    cxString atlasData = cxEngineAssetsData(atlas);
    if(!cxStringOK(atlasData)){
        CX_ERROR("get atlas file %s data failed",atlas);
        return false;
    }
    this->atlas = spAtlas_create(cxStringBody(atlasData), cxStringLength(atlasData), "", this);
    if(this->atlas == NULL){
        CX_ERROR("create atlas data failed");
        return false;
    }
    spSkeletonJson* spJson = spSkeletonJson_create(this->atlas);
    if(spJson == NULL){
        CX_ERROR("create skeletion json failed");
        return false;
    }
    spJson->scale = scale;
    cxString jsonData = cxEngineAssetsData(json);
    if(cxStringOK(jsonData)) {
        this->skeletonData = spSkeletonJson_readSkeletonData(spJson, cxStringBody(jsonData));
    }
    spSkeletonJson_dispose(spJson);
    if(this->skeletonData == NULL){
        CX_ERROR("read json data to skeleton failed");
        return false;
    }
    this->skeleton = spSkeleton_create(this->skeletonData);
    if(this->skeleton == NULL){
        CX_ERROR("create skeleton failed");
        return false;
    }
	this->rootBone = this->skeleton->root;
    this->stateData = spAnimationStateData_create(this->skeletonData);
    if(this->stateData == NULL){
        CX_ERROR("create animation state data error");
        return false;
    }
    if(mixs != NULL){
        cxSpineSetMixs(this, mixs);
    }
    return true;
}













