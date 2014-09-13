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
    spRegionAttachment_computeWorldVertices(self, slot->bone, this->worldVertices);
    
    color.a = this->skeleton->a * self->a * slot->a;
    color.r = this->skeleton->r * self->r * slot->r;
    color.g = this->skeleton->g * self->g * slot->g;
    color.b = this->skeleton->b * self->b * slot->b;
    
    bp.lb.colors = color;
    bp.rb.colors = color;
    bp.lt.colors = color;
    bp.rt.colors = color;
    
    bp.lb.vertices = cxVec3fv(this->worldVertices[SP_VERTEX_X1], this->worldVertices[SP_VERTEX_Y1], 0);
    bp.lt.vertices = cxVec3fv(this->worldVertices[SP_VERTEX_X2], this->worldVertices[SP_VERTEX_Y2], 0);
    bp.rt.vertices = cxVec3fv(this->worldVertices[SP_VERTEX_X3], this->worldVertices[SP_VERTEX_Y3], 0);
    bp.rb.vertices = cxVec3fv(this->worldVertices[SP_VERTEX_X4], this->worldVertices[SP_VERTEX_Y4], 0);
    
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
	spAnimationState_update(this->state, engine->frameDelta);
	spAnimationState_apply(this->state, this->skeleton);
	spSkeleton_updateWorldTransform(this->skeleton);

    cxTexture texture = NULL;
    GLenum dfactor = 0;
    GLenum sfactor = 0;
    cxAtlasClean(this);
	for (cxInt i = 0; i < this->skeleton->slotsCount; i++) {
		spSlot* slot = this->skeleton->drawOrder[i];
        if(slot->attachment == NULL || slot->attachment->type != SP_ATTACHMENT_REGION){
            continue;
        }
        spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
        if(texture == NULL){
            texture = ((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
            dfactor = slot->data->additiveBlending ? GL_ONE : this->cxAtlas.cxSprite.dfactor;
            sfactor = this->cxAtlas.cxSprite.sfactor;
        }
        if(texture == NULL){
            continue;
        }
        cxSpineUpdateBox(this, attachment, slot);
	}
    cxSpriteSetBlendFactor(this, sfactor, dfactor);
    cxSpriteSetTexture(this, texture);
}

CX_OBJECT_TYPE(cxSpine, cxAtlas)
{
    
}
CX_OBJECT_INIT(cxSpine, cxAtlas)
{
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onUpdate, cxSpineUpdate);

    cxAtlasSetCapacity(this, 256);
    this->worldVertices = allocator->malloc(sizeof(cxFloat) * 1000);
    
    cxString aData = cxEngineAssetsData("spineboy.atlas");

    this->atlas = spAtlas_create(cxStringBody(aData), cxStringLength(aData), "", this);
    
    spSkeletonJson* json = spSkeletonJson_create(this->atlas);
    json->scale = 1.0f;
    
    aData = cxEngineAssetsData("spineboy.json");
    this->skeletonData = spSkeletonJson_readSkeletonData(json, cxStringBody(aData));
    spSkeletonJson_dispose(json);
    
    this->skeleton = spSkeleton_create(this->skeletonData);
	this->rootBone = this->skeleton->root;
    
    //create animate
    this->state = spAnimationState_create(spAnimationStateData_create(this->skeleton->data));
    spAnimationStateData_setMixByName(this->state->data, "walk", "jump", 0.2f);
    spAnimationStateData_setMixByName(this->state->data, "jump", "run", 0.4f);
    spAnimation* animation = spSkeletonData_findAnimation(this->skeleton->data, "walk");
	if(animation != NULL){
        spAnimationState_setAnimation(this->state, 10, animation, true);
    }
    animation = spSkeletonData_findAnimation(this->skeleton->data, "jump");
    if(animation != NULL){
        spAnimationState_addAnimation(this->state, 10, animation, true, 3);
    }
    animation = spSkeletonData_findAnimation(this->skeleton->data, "run");
    if(animation != NULL){
        spAnimationState_addAnimation(this->state, 10, animation, true, 0);
    }
	this->state->rendererObject = this;
}
CX_OBJECT_FREE(cxSpine, cxAtlas)
{
    if(this->state != NULL){
        spAnimationState_dispose(this->state);
    }
    if(this->worldVertices != NULL){
        allocator->free(this->worldVertices);
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













