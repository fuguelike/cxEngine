//
//  cxAtlas.c
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureCache.h>
#include <engine/cxEngine.h>
#include <engine/cxFrames.h>
#include "cxAtlas.h"

static void cxAtlasDrawBorder(cxAtlas this)
{
    cxColor3f color = cxViewGetBorderColor(this);
    for(cxInt i=0; i < this->number; i++){
        cxBoxPoint *bp = &this->boxes[i];
        cxBox4f b = cxBox4fv(bp->lb.vertices.x, bp->rb.vertices.x, bp->lt.vertices.y, bp->rb.vertices.y);
        cxBoxVec2f box = cxBoxVec2fFromBox4f(b);
        cxDrawLineBox(&box, color);
    }
}

CX_METHOD_DEF(cxAtlas,OnDraw,void)
{
    cxOpenGL gl = cxOpenGLInstance();
    if(this->number == 0 || !cxSpriteBindTexture(this)){
        return;
    }
    cxAtlasDrawInit(this);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
    if(this->isDirty){
        glBufferSubData(GL_ARRAY_BUFFER, 0 ,sizeof(cxBoxPoint) * this->number, this->boxes);
        this->isDirty = false;
    }
    if(gl->support_GL_OES_vertex_array_object){
        glBindVertexArray(this->vaoid);
        glDrawElements(GL_TRIANGLES, this->number*6, GL_UNSIGNED_SHORT, NULL);
        glBindVertexArray(0);
    }else{
        //position
        glEnableVertexAttribArray(cxVertexAttribPosition);
        glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, vertices));
        //colors
        glEnableVertexAttribArray(cxVertexAttribColor);
        glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, colors));
        //texcoords
        glEnableVertexAttribArray(cxVertexAttribTexcoord);
        glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, texcoords));
        //
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboid[1]);
        glDrawElements(GL_TRIANGLES, this->number*6, GL_UNSIGNED_SHORT, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(this->border){
        cxAtlasDrawBorder(this);
    }
}

void cxAtlasSetDirty(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    this->isDirty = true;
}

void cxAtlasSetScale9(cxAny pview,cxBox4f box)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    this->scale9.enable = true;
    if(cxBox4fEqu(this->scale9.box, box)){
        return;
    }
    this->scale9.box = box;
    cxViewSetDirty(this, cxViewDirtyTexture);
}

void cxAtlasUpdateScale9(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_RETURN(!this->scale9.enable || this->cxSprite.Texture == NULL || cxViewZeroSize(pview));
    cxAtlasClear(pview);
    cxSize2f size = cxViewGetSize(this);
    if(size.w < (this->scale9.box.l + this->scale9.box.r)){
        size.w = this->scale9.box.l + this->scale9.box.r;
    }
    if(size.h < (this->scale9.box.t + this->scale9.box.b)){
        size.h = this->scale9.box.t + this->scale9.box.b;
    }
    cxViewSetSize(this, size);
    cxTexCoord coord = cxSpriteGetCoord(this);
    cxSize2f texsiz = this->cxSprite.Texture->size;
    CX_RETURN(coord == NULL);
    cxFloat tx,ty,tw,th;
    if(coord->rotated){
        tx = coord->frame.y / texsiz.h;
        ty = (coord->frame.x + coord->frame.h) / texsiz.w;
        tw = coord->frame.w / texsiz.w;
        th = coord->frame.h / texsiz.h;
    }else{
        tx = coord->frame.x / texsiz.w;
        ty = coord->frame.y / texsiz.h;
        tw = coord->frame.w / texsiz.w;
        th = coord->frame.h / texsiz.h;
    }
    cxSize2f tsize = cxSize2fv(coord->frame.w, coord->frame.h);
    cxFloat txs[]={0.0f, this->scale9.box.l/tsize.w, (tsize.w - this->scale9.box.r)/tsize.w, 1.0f};
    cxFloat tys[]={0.0f, this->scale9.box.t/tsize.h, (tsize.h - this->scale9.box.b)/tsize.h, 1.0f};
    if(coord->rotated){
        for(int i=0; i < 4; i++){
            txs[i] = txs[i] * tw + tx;
            tys[i] = ty - tys[i] * th;
        }
    }else{
        for(int i=0; i < 4; i++){
            txs[i] = txs[i] * tw + tx;
            tys[i] = tys[i] * th + ty;
        }
    }
    cxFloat bxs[]={0.0f, this->scale9.box.l, size.w - this->scale9.box.r, size.w};
    cxFloat bys[]={0.0f, this->scale9.box.t, size.h - this->scale9.box.b, size.h};
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
        if(cxFloatEqu(hw, 0) || cxFloatEqu(hh, 0)){
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
        if(coord->rotated){
            bp.lb.texcoords = cxTex2fv(ty2, tx1);
            bp.rb.texcoords = cxTex2fv(ty2, tx2);
            bp.lt.texcoords = cxTex2fv(ty1, tx1);
            bp.rt.texcoords = cxTex2fv(ty1, tx2);
        }else{
            bp.lb.texcoords = cxTex2fv(tx1, ty2);
            bp.rb.texcoords = cxTex2fv(tx2, ty2);
            bp.lt.texcoords = cxTex2fv(tx1, ty1);
            bp.rt.texcoords = cxTex2fv(tx2, ty1);
        }
        cxAtlasAppend(this, &bp);
    }
}
CX_SETTER_DEF(cxAtlas, frames)
{
    cxAny pobj = cxJsonMakeObject(value);
    CX_ASSERT_VALUE(pobj, cxFrames, fs);
    CX_RETAIN_SWAP(this->frames, fs);
}
CX_SETTER_DEF(cxAtlas, border)
{
    cxBool bv = cxJsonToBool(value, false);
    cxViewSetShowBorder(this, bv);
    this->border = bv;
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
    cxAtlasSetScale9(this, cxJsonToBox4f(value, this->scale9.box));
}
CX_SETTER_DEF(cxAtlas, frameidx)
{
    this->FrameIdx = cxJsonToInt(value, this->FrameIdx);
}
CX_METHOD_DEF(cxAtlas, OnDirty, void)
{
    CX_SUPER(cxSprite, this, OnDirty, CX_M(void));
    cxViewDirty dirty = cxViewGetDirty(this);
    if((dirty & cxViewDirtySize) || (dirty & cxViewDirtyTexture) ||(dirty & cxViewDirtyColor)){
        cxAtlasUpdateScale9(this);
    }
    if((dirty & cxViewDirtyTexture) && this->frames != NULL){
        cxAtlasSetFrames(this, this->frames, this->FrameIdx, NULL, 0);
    }
}
CX_TYPE(cxAtlas, cxSprite)
{
    CX_SETTER(cxAtlas, border);
    CX_SETTER(cxAtlas, frames);
    CX_SETTER(cxAtlas, blend);
    CX_SETTER(cxAtlas, scale9);
    CX_SETTER(cxAtlas, frameidx);
    
    CX_METHOD(cxAtlas, OnDraw);
    CX_METHOD(cxAtlas, OnDirty);
}
CX_INIT(cxAtlas, cxSprite)
{
    glGenVertexArrays(1, &this->vaoid);
    glGenBuffers(2, this->vboid);
}
CX_FREE(cxAtlas, cxSprite)
{
    CX_RELEASE(this->frames);
    allocator->free(this->boxes);
    allocator->free(this->indices);
    glDeleteBuffers(2, this->vboid);
    glDeleteVertexArrays(1, &this->vaoid);
}
CX_TERM(cxAtlas, cxSprite)

cxInt cxAtlasAppendBoxPointWithKey(cxAny pview,cxVec2f pos,cxSize2f size,cxColor4f color,cxBool flipx,cxBool flipy,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    cxBoxTex2f box = cxSpriteBoxTexWithKey(this, key ,flipx,flipy);
    return cxAtlasAppendBoxPoint(this, pos, size, box, color);
}

cxInt cxAtlasAppendBoxPoint(cxAny pview,cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color)
{
    cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
    return cxAtlasAppend(pview, &bp);
}

void cxAtlasAppendEmpty(cxAny pview)
{
    cxBoxPoint bp = {0};
    cxAtlasAppend(pview, &bp);
}

cxInt cxAtlasAppendWithKey(cxAny pview,cxVec2f pos,cxSize2f size,cxColor4f color, cxBool flipx,cxBool flipy,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    cxBoxPoint bp={0};
    cxAtlasSetBoxPoint(this, &bp, pos, size, color, flipx, flipy, key);
    return cxAtlasAppend(this, &bp);
}

void cxTexCoordSetBoxPoint(cxAny pview,cxBoxPoint *bp,cxVec2f pos,cxSize2f size,cxColor4f color, cxBool flipx,cxBool flipy,cxTexCoord coord)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    cxTexture texture = cxSpriteGetTexture(this);
    CX_ASSERT(texture != NULL, "texture null");
    cxFloat wh = size.w/2.0f;
    cxFloat hh = size.h/2.0f;
    cxVec2f scale = cxVec2fv(1.0f, 1.0f);
    cxSize2f vsiz = cxViewGetSize(this);
    if(!cxFloatEqu(vsiz.w, 0)){
        scale.x = vsiz.w / size.w;
    }
    if(!cxFloatEqu(vsiz.h, 0)){
        scale.y = vsiz.h / size.h;
    }
    cxBoxTex2f tbox = cxTextureBoxCoord(texture, coord, cxBox4fx(0), flipx, flipy);
    cxBox4f vbox = cxBox4fv(-wh, +wh, +hh, -hh);
    vbox = cxTexCoordTrimmedFix(coord, vbox, size, pos , scale, flipx, flipy);
    
    cxFloat Z = cxViewGetZ(this);
    
    bp->lb.colors = color;
    bp->lb.texcoords = tbox.lb;
    bp->lb.vertices = cxVec3fv(vbox.l, vbox.b, Z);
    
    bp->lt.colors = color;
    bp->lt.texcoords = tbox.lt;
    bp->lt.vertices = cxVec3fv(vbox.l, vbox.t, Z);
    
    bp->rt.colors = color;
    bp->rt.texcoords = tbox.rt;
    bp->rt.vertices = cxVec3fv(vbox.r, vbox.t, Z);
    
    bp->rb.colors = color;
    bp->rb.texcoords = tbox.rb;
    bp->rb.vertices = cxVec3fv(vbox.r, vbox.b, Z);
}

cxTexCoord cxAtlasSetBoxPoint(cxAny pview,cxBoxPoint *bp,cxVec2f pos,cxSize2f size,cxColor4f color, cxBool flipx,cxBool flipy,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(cxConstCharsOK(key), "args key error");
    cxTexture texture = cxSpriteGetTexture(this);
    if(texture == NULL){
        CX_WARN("texture NULL");
        return NULL;
    }
    cxTexCoord coord = cxTextureCoord(texture, key);
    if(coord == NULL){
        CX_WARN("get coord key %s null",key);
        return NULL;
    }
    cxTexCoordSetBoxPoint(pview, bp, pos, size, color, flipx, flipy, coord);
    return coord;
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

// = cxAtlasKeep(pview,0)
void cxAtlasClear(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    cxAtlasSetNumber(this, 0);
}

cxInt cxAtlasAppend(cxAny pview,cxBoxPoint *point)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    //realloc
    if(this->capacity <= this->number){
        cxAtlasSetCapacity(pview, this->capacity + 8);
    }
    CX_ASSERT(this->number < this->capacity, "atlas number > boxNumber");
    this->boxes[this->number++] = *point;
    this->isDirty = true;
    return this->number - 1;
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
    this->isDirty = true;
}

void cxAtlasShowAt(cxAny pview,cxInt index,cxBool show)
{
    cxAtlasUpdateAlpha(pview, index, show?1.0f:0.0f);
}

void cxAtlasUpdateTexture(cxAny pview,cxInt index,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(index >= 0 && index < this->number, "index > boxNumber");
    cxTexture texture = cxSpriteGetTexture(this);
    cxBoxTex2f boxtex = cxTextureBox(texture, key);
    cxAtlasUpdateBoxTex(this, index, boxtex);
}

void cxAtlasUpdateBoxTex(cxAny pview,cxInt index,cxBoxTex2f boxtex)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(index >= 0 && index < this->number, "index > boxNumber");
    cxBoxPoint *bp = &this->boxes[index];
    bp->lb.texcoords = boxtex.lb;
    bp->lt.texcoords = boxtex.lt;
    bp->rt.texcoords = boxtex.rt;
    bp->rb.texcoords = boxtex.rb;
    this->isDirty = true;
}

void cxAtlasUpdateAlpha(cxAny pview,cxInt index,cxFloat alpha)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(index >= 0 && index < this->number, "index > boxNumber");
    cxBoxPoint *bp = &this->boxes[index];
    bp->lb.colors.a = alpha;
    bp->lt.colors.a = alpha;
    bp->rt.colors.a = alpha;
    bp->rb.colors.a = alpha;
    this->isDirty = true;
}

void cxAtlasUpdateColor(cxAny pview,cxInt index,cxColor4f color)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(index >= 0 && index < this->number, "index > boxNumber");
    cxBoxPoint *bp = &this->boxes[index];
    bp->lb.colors = color;
    bp->lt.colors = color;
    bp->rt.colors = color;
    bp->rb.colors = color;
    this->isDirty = true;
}

void cxAtlasUpdatePosSize(cxAny pview,cxInt index,cxVec2f pos,cxSize2f size)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(index >= 0 && index < this->number, "index > boxNumber");
    cxBoxPoint *bp = &this->boxes[index];
    cxFloat wh = size.w/2.0f;
    cxFloat hh = size.h/2.0f;
    bp->lb.vertices = cxVec3fv(pos.x - wh, pos.y - hh, 0.0f);
    bp->lt.vertices = cxVec3fv(pos.x - wh, pos.y + hh, 0.0f);
    bp->rt.vertices = cxVec3fv(pos.x + wh, pos.y + hh, 0.0f);
    bp->rb.vertices = cxVec3fv(pos.x + wh, pos.y - hh, 0.0f);
    this->isDirty = true;
}

void cxAtlasUpdateAt(cxAny pview,cxInt index, cxBoxPoint *point)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(index >= 0 && index < this->capacity, "index > boxNumber");
    CX_RETURN(memcmp(&this->boxes[index], point, sizeof(cxBoxPoint)) == 0);
    this->boxes[index] = *point;
    this->isDirty = true;
}

void cxAtlasSetNumber(cxAny pview,cxInt number)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT(number >= 0 && number <= this->capacity, "number must >= 0 < capacity");
    CX_RETURN(this->number == number);
    this->number = number;
}

void cxAtlasDrawInit(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    if(this->isInit){
        return;
    }
    cxOpenGL gl = cxOpenGLInstance();
    if(gl->support_GL_OES_vertex_array_object){
        glBindVertexArray(this->vaoid);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cxBoxPoint) * this->capacity, this->boxes, GL_DYNAMIC_DRAW);
        //vertices
        glEnableVertexAttribArray(cxVertexAttribPosition);
        glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, vertices));
        //colors
        glEnableVertexAttribArray(cxVertexAttribColor);
        glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, colors));
        //tex coords
        glEnableVertexAttribArray(cxVertexAttribTexcoord);
        glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, texcoords));
        //
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboid[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cxIndices)*this->capacity, this->indices, GL_STATIC_DRAW);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }else{
        glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cxBoxPoint) * this->capacity, this->boxes, GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboid[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cxIndices) * this->capacity, this->indices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    this->isInit = true;
}

cxAny cxAtlasCreate(cxConstChars url)
{
    cxAtlas this = CX_CREATE(cxAtlas);
    cxSpriteSetTextureURL(this, url);
    return this;
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



