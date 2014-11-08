//
//  cxAtlas.c
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureCache.h>
#include <engine/cxEngine.h>
#include "cxAtlas.h"

static void CX_METHOD(cxAtlas,Draw)
{
    CX_RETURN(this->number == 0 || this->cxSprite.Texture == NULL);
    cxOpenGLSetBlendFactor(this->cxSprite.sfactor, this->cxSprite.dfactor);
    cxShaderUsing(this->cxSprite.Shader);
    cxTextureFireBind(this->cxSprite.Texture);
    if(!this->isInit){
        this->isInit = true;
        cxAtlasDrawInit(this);
    }
    if(cxOpenGLInstance()->support_GL_OES_vertex_array_object){
        cxOpenGLVAODraw(this->vaoid, this->vboid, this->number, this->boxes, &this->isDirty);
    }else{
        cxOpenGLVBODraw(this->vboid, this->number, this->boxes, &this->isDirty);
    }
}

void cxAtlasSetScale9(cxAny pview,cxBox4f box)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    this->scale9.box = box;
    this->scale9.enable = true;
    cxAtlasUpdateScale9(pview);
}

void cxAtlasUpdateScale9(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_RETURN(!this->scale9.enable);
    CX_ASSERT(!cxViewZeroSize(pview) && this->cxSprite.Texture != NULL, "must set texture and size");
    cxAtlasClear(pview);
    cxSize2f size = cxViewGetSize(this);
    cxRect4f tr = cxBoxTex2fToRect4f(this->cxSprite.texCoord);
    cxSize2f tsize = cxSize2fv(this->cxSprite.Texture->size.w * tr.w, this->cxSprite.Texture->size.h * tr.h);
    cxFloat txs[]={0.0f, this->scale9.box.l/tsize.w, (tsize.w - this->scale9.box.r)/tsize.w, 1.0f};
    cxFloat tys[]={0.0f, this->scale9.box.t/tsize.h, (tsize.h - this->scale9.box.b)/tsize.h, 1.0f};
    for(int i=0; i < 4; i++){
        txs[i] = txs[i] * tr.w + tr.x;
        tys[i] = tys[i] * tr.h + tr.y;
    }
    cxFloat bxs[]={0.0f, this->scale9.box.l, size.w - this->scale9.box.r, size.w};
    cxFloat bys[]={0.0f, this->scale9.box.r, size.h - this->scale9.box.b, size.h};
    cxFloat tx1=0,ty1=0,tx2=0,ty2=0;
    cxFloat bx1=0,by1=0,bx2=0,by2=0;
    cxFloat offx=0,offy=0;
    cxColor4f color =cxViewGetColor(this);
    for(int i=0; i < 9;i++){
        tx1 = txs[i%3];
        tx2 = txs[i%3 + 1];
        ty1 = tys[i/3];
        ty2 = tys[i/3 + 1];
        bx1 = bxs[i%3];
        bx2 = bxs[i%3 + 1];
        by1 = bys[i/3];
        by2 = bys[i/3 + 1];
        cxFloat hw = (bx2 - bx1)/2.0f;
        cxFloat hh = (by2 - by1)/2.0f;
        if(hw == 0 || hh == 0){
            continue;
        }
        offx = (bx2 - bx1)/2.0f + bx1 - size.w/2.0f;
        offy = (size.h - (by2 - by1))/2.0f - by1;
        cxBoxPoint bp;
        bp.lb.colors = color;
        bp.lt.colors = color;
        bp.rb.colors = color;
        bp.rt.colors = color;
        bp.lb.vertices = cxVec3fv(-hw + offx, -hh + offy, 0.0f);
        bp.rb.vertices = cxVec3fv( hw + offx, -hh + offy, 0.0f);
        bp.lt.vertices = cxVec3fv(-hw + offx,  hh + offy, 0.0f);
        bp.rt.vertices = cxVec3fv( hw + offx,  hh + offy, 0.0f);
        bp.lb.texcoords = cxTex2fv(tx1, ty2);
        bp.rb.texcoords = cxTex2fv(tx2, ty2);
        bp.lt.texcoords = cxTex2fv(tx1, ty1);
        bp.rt.texcoords = cxTex2fv(tx2, ty1);
        cxAtlasAppend(this, &bp);
    }
}

void cxAtlasResize(cxAny pview)
{
    cxAtlasUpdateScale9(pview);
}

static void cxAtlasDirty(cxAny pview)
{
    if(cxViewGetDirty(pview) & cxViewDirtySize){
        cxAtlasResize(pview);
    }
}

CX_SETTER_DEF(cxAtlas, blend)
{
    cxConstChars blend = cxJsonToConstChars(value);
    if(cxConstCharsEqu(blend, "add")){
        cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE);
    }else if(cxConstCharsEqu(blend, "multiply")){
        cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }else{
        CX_ASSERT_FALSE("not support mode %s",blend);
    }
}
CX_SETTER_DEF(cxAtlas, scale9)
{
    if(cxJsonBool(value, "enable", false)){
        cxAtlasSetCapacity(this, 9);
        this->scale9.enable = true;
        this->scale9.box = cxJsonBox4f(value, "box", this->scale9.box);
        cxAtlasUpdateScale9(this);
    }
}

CX_TYPE(cxAtlas, cxSprite)
{
    CX_SETTER(cxAtlas, blend);
    CX_SETTER(cxAtlas, scale9);
    
    CX_MSET(cxAtlas, Draw);
}
CX_INIT(cxAtlas, cxSprite)
{
    this->isDirty = true;
    glGenVertexArrays(1, &this->vaoid);
    glGenBuffers(2, this->vboid);
    CX_ADD(cxView, this, onDirty, cxAtlasDirty);
    this->items = CX_ALLOC(cxHash);
}
CX_FREE(cxAtlas, cxSprite)
{
    CX_RELEASE(this->items);
    allocator->free(this->boxes);
    allocator->free(this->indices);
    glDeleteBuffers(2, this->vboid);
    glDeleteVertexArrays(1, &this->vaoid);
}
CX_TERM(cxAtlas, cxSprite)

void cxAtlasAppendBoxPoint(cxAny pview,cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color)
{
    cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
    cxAtlasAppend(pview, &bp);
}

void cxAtlasAppendEmpty(cxAny pview)
{
    cxBoxPoint bp = {0};
    cxAtlasAppend(pview, &bp);
}

cxBoxPoint cxAtlasCreateBoxPoint(cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color)
{
    cxBoxPoint rv;
    cxFloat wh = size.w/2.0f;
    cxFloat hh = size.h/2.0f;

    rv.lb.colors = color;
    rv.lb.texcoords = tex.lb;
    rv.lb.vertices = cxVec3fv(pos.x - wh, pos.y - hh, 0.0f);

    rv.lt.colors = color;
    rv.lt.texcoords = tex.lt;
    rv.lt.vertices = cxVec3fv(pos.x - wh, pos.y + hh, 0.0f);

    rv.rt.colors = color;
    rv.rt.texcoords = tex.rt;
    rv.rt.vertices = cxVec3fv(pos.x + wh, pos.y + hh, 0.0f);

    rv.rb.colors = color;
    rv.rb.texcoords = tex.rb;
    rv.rb.vertices = cxVec3fv(pos.x + wh, pos.y - hh, 0.0f);
    return rv;
}

void cxAtlasClear(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    this->number = 0;
    this->isDirty = true;
}

void cxAtlasAppend(cxAny pview,cxBoxPoint *point)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    //realloc
    if(this->capacity <= this->number){
        cxAtlasSetCapacity(pview, this->capacity + 8);
    }
    CX_ASSERT(this->number < this->capacity, "atlas number > boxNumber");
    this->boxes[this->number++] = *point;
    this->isDirty = true;
}

void cxAtlasFastRemove(cxAny pview,cxInt index)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(index >= 0 && index < this->number, "index > boxNumber");
    cxInt lastIdx = this->number - 1;
    if(index != lastIdx){
        this->boxes[index] = this->boxes[lastIdx];
    }
    this->number--;
}

void cxAtlasUpdateAt(cxAny pview,cxInt index, cxBoxPoint *point)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(index >= 0 && index < this->number, "index > boxNumber");
    this->boxes[index] = *point;
    this->isDirty = true;
}

void cxAtlasSetNumber(cxAny pview,cxInt number)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(number >= 0 && number <= this->capacity, "number must >= 0 < capacity");
    this->isDirty = true;
    this->number = number;
}

void cxAtlasSetDirty(cxAny pview,cxBool v)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    this->isDirty = v;
}

void cxAtlasDrawInit(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    if(cxOpenGLInstance()->support_GL_OES_vertex_array_object){
        cxOpenGLInitVAO(this->vaoid, this->vboid, this->capacity, this->boxes, this->indices);
    }else{
        cxOpenGLInitVBO(this->vboid, this->capacity, this->boxes, this->indices);
    }
}

void cxAtlasSetCapacity(cxAny pview,cxInt capacity)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_RETURN(this->capacity >= capacity);
    this->capacity = capacity;
    cxInt size = this->capacity * sizeof(cxBoxPoint);
    //
    this->boxes = allocator->realloc(this->boxes,size);
    CX_ASSERT(this->boxes != NULL, "out of memory");
    //
    size = this->capacity * sizeof(cxIndices);
    this->indices = allocator->realloc(this->indices,size);
    CX_ASSERT(this->indices != NULL, "out of memory");
    //
    for(int i=0; i < this->capacity;i++){
        this->indices[i*6+0] = i*4+0;
        this->indices[i*6+1] = i*4+1;
        this->indices[i*6+2] = i*4+2;
        this->indices[i*6+3] = i*4+3;
        this->indices[i*6+4] = i*4+2;
        this->indices[i*6+5] = i*4+1;
    }
    this->isInit = false;
}



