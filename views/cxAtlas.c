//
//  cxAtlas.c
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include <core/cxEngine.h>
#include "cxAtlas.h"

static void cxAtlasVAODraw(void *pview)
{
    cxAtlas this = pview;
    if (this->isDirty){
        glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cxBoxPoint) * this->number, this->boxes);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        this->isDirty = false;
    }
    glBindVertexArray(this->vaoid);
    glDrawElements(GL_TRIANGLES, this->number*6, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}

static void cxAtlasVBODraw(cxAny pview)
{
    cxAtlas this = pview;
    
    glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
    if (this->isDirty){
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cxBoxPoint) * this->number, this->boxes);
        this->isDirty = false;
    }
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, vertices));
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, texcoords));
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, colors));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboid[1]);
    glDrawElements(GL_TRIANGLES, (GLsizei)this->number*6, GL_UNSIGNED_SHORT, (GLvoid*)NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void cxAtlasDraw(cxAny pview)
{
    cxAtlas this = pview;
    CX_RETURN(this->number == 0 || this->super.texture == NULL);
    cxOpenGLSetBlendFactor(this->super.sfactor, this->super.dfactor);
    cxShaderUsing(this->super.shader,this->super.texture->isAtlas);
    cxTextureBind(this->super.texture);
    if(!this->isInit){
        this->isInit = true;
        cxAtlasDrawInit(pview);
    }
    if(cxOpenGLInstance()->support_GL_OES_vertex_array_object){
        cxAtlasVAODraw(pview);
    }else{
        cxAtlasVBODraw(pview);
    }
}

void cxAtlasSetScale9(cxAny pview,cxBox4f box)
{
    cxAtlas this = pview;
    this->scale9.box = box;
    this->scale9.enable = true;
    cxAtlasUpdateScale9(pview);
}

void cxAtlasUpdateScale9(cxAny pview)
{
    cxAtlas this = pview;
    CX_RETURN(!this->scale9.enable);
    CX_ASSERT(!cxViewZeroSize(pview) && this->super.texture != NULL, "must set texture and size");
    cxAtlasClean(pview);
    cxSize2f size = cxViewSize(this);
    cxRect4f tr = cxBoxTex2fToRect4f(this->super.texCoord);
    cxSize2f tsize = cxSize2fv(this->super.texture->size.w * tr.w, this->super.texture->size.h * tr.h);
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
    cxColor4f color =cxViewColor(this);
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
        cxAtlasAppend(this, bp);
    }
}

void cxAtlasResize(cxEvent *event)
{
    cxAtlasUpdateScale9(event->sender);
}

void __cxAtlasInitObject(cxAny object,cxAny json)
{
    CX_BASE_SUPER(cxSprite);
    cxAtlas this = object;
    cxJson points = cxJsonArray(json, "points");
    if(cxJsonBool(json, "scale9.enable", false)){
        
        cxAtlasSetCapacity(this, 9);
        this->scale9.enable = true;
        this->scale9.box.l = cxJsonDouble(json, "scale9.box.l", this->scale9.box.l);
        this->scale9.box.r = cxJsonDouble(json, "scale9.box.r", this->scale9.box.r);
        this->scale9.box.t = cxJsonDouble(json, "scale9.box.t", this->scale9.box.t);
        this->scale9.box.b = cxJsonDouble(json, "scale9.box.b", this->scale9.box.b);
        cxAtlasUpdateScale9(this);
    }else if(points != NULL){
        cxInt len = cxJsonArrayLength(points);
        CX_ASSERT(len > 0, "points empty");
        cxAtlasSetCapacity(this, len);
        CX_JSON_ARRAY_EACH_BEG(points, point);
        {
            cxVec2f pos = cxVec2fv(0, 0);
            cxSize2f size = cxSize2fv(0, 0);
            cxBoxTex2f tex = cxBoxTex2fDefault();
            cxColor4f color = cxColor4fv(1, 1, 1, 1);
            pos.x = cxJsonDouble(point, "p.x", pos.x);
            pos.y = cxJsonDouble(point, "p.y", pos.y);
            size.w = cxJsonDouble(point, "s.w", size.w);
            size.h = cxJsonDouble(point, "s.h", size.h);
            tex.lb.u = cxJsonDouble(point, "t.lb.u", tex.lb.u);
            tex.lb.v = cxJsonDouble(point, "t.lb.v", tex.lb.v);
            tex.rb.u = cxJsonDouble(point, "t.rb.u", tex.rb.u);
            tex.rb.v = cxJsonDouble(point, "t.rb.v", tex.rb.v);
            tex.lt.u = cxJsonDouble(point, "t.lt.u", tex.lt.u);
            tex.lt.v = cxJsonDouble(point, "t.lt.v", tex.lt.v);
            tex.rt.u = cxJsonDouble(point, "t.rt.u", tex.rt.u);
            tex.rt.v = cxJsonDouble(point, "t.rt.v", tex.rt.v);
            color.a = cxJsonDouble(point, "c.a", color.a);
            color.r = cxJsonDouble(point, "c.r", color.r);
            color.g = cxJsonDouble(point, "c.g", color.g);
            color.b = cxJsonDouble(point, "c.b", color.b);
            cxAtlasAppendBoxPoint(this, pos, size, tex, color);
        }
        CX_JSON_ARRAY_EACH_END(points, point);
    }
}

CX_OBJECT_INIT(cxAtlas, cxSprite)
{
    CX_BASE_OVERRIDE(cxAtlas, this);
    this->isDirty = true;
    glGenVertexArrays(1, &this->vaoid);
    glGenBuffers(2, this->vboid);
    CX_METHOD_OVERRIDE(this->super.super.Draw, cxAtlasDraw);
    CX_EVENT_QUICK(this->super.super.onResize, cxAtlasResize);
    this->items = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxAtlas, cxSprite)
{
    CX_RELEASE(this->items);
    allocator->free(this->boxes);
    allocator->free(this->indices);
    glDeleteBuffers(2, this->vboid);
    glDeleteVertexArrays(1, &this->vaoid);
}
CX_OBJECT_TERM(cxAtlas, cxSprite)

void cxAtlasAppendBoxPoint(cxAny pview,cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color)
{
    cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
    cxAtlasAppend(pview, bp);
}

cxInt cxAtlasAppendSprite(cxAny pview,cxConstChars key)
{
    cxAtlas this = pview;
    cxColor4f color = cxColor4fv(1, 1, 1, 1);
    cxVec2f pos = cxVec2fv(0, 0);
    cxBoxTex2f tbox = cxTextureBox(this->super.texture, key);
    cxSize2f siz = cxTextureSize(this->super.texture, key);
    cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, siz, tbox, color);
    cxAtlasAppend(pview, bp);
    return this->number - 1;
}

void cxAtlasSetItemPos(cxAny pview,cxInt idx,cxVec2f pos)
{
    cxAtlas this = pview;
    cxBoxPoint *bp = &this->boxes[idx];
    cxFloat wh = (bp->rb.vertices.x - bp->lt.vertices.x) / 2.0f;
    cxFloat hh = (bp->lt.vertices.y - bp->rb.vertices.y) / 2.0f;
    bp->lb.vertices = cxVec3fv(pos.x - wh, pos.y - hh, 0.0f);
    bp->lt.vertices = cxVec3fv(pos.x - wh, pos.y + hh, 0.0f);
    bp->rt.vertices = cxVec3fv(pos.x + wh, pos.y + hh, 0.0f);
    bp->rb.vertices = cxVec3fv(pos.x + wh, pos.y - hh, 0.0f);
    this->isDirty = true;
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

void cxAtlasClean(cxAny pview)
{
    cxAtlas this = pview;
    this->number = 0;
    this->isDirty = true;
}

void cxAtlasAppend(cxAny pview,cxBoxPoint point)
{
    cxAtlas this = pview;
    //realloc
    if(this->capacity <= this->number){
        cxAtlasSetCapacity(pview, this->capacity + 8);
    }
    CX_ASSERT(this->number < this->capacity, "atlas number > boxNumber");
    this->boxes[this->number++] = point;
    this->isDirty = true;
}

void cxAtlasUpdate(cxAny pview,cxInt index, cxBoxPoint point)
{
    cxAtlas this = pview;
    CX_ASSERT(index >= 0 && index < this->capacity, "index > boxNumber");
    this->boxes[index] = point;
    this->isDirty = true;
}

static void cxAtlasInitVAO(cxAny pview)
{
    cxAtlas this = pview;
    
    glBindVertexArray(this->vaoid);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cxBoxPoint) * this->capacity, this->boxes, GL_DYNAMIC_DRAW);

    //vertices
    glEnableVertexAttribArray(cxVertexAttribPosition);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE,sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, vertices));
    //colors
    glEnableVertexAttribArray(cxVertexAttribColor);
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE,sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, colors));
    //tex coords
    glEnableVertexAttribArray(cxVertexAttribTexcoord);
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE,sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, texcoords));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboid[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*this->capacity*6, this->indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void cxAtlasInitVBO(cxAny pview)
{
    cxAtlas this = pview;
    
    glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cxBoxPoint) * this->capacity, this->boxes, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboid[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * this->capacity * 6, this->indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cxAtlasSetNumber(cxAny pview,cxInt number)
{
    CX_ASSERT(number >= 0, "number must >= 0");
    cxAtlas this = pview;
    this->isDirty = true;
    this->number = number;
}

void cxAtlasDrawInit(cxAny pview)
{
    if(cxOpenGLInstance()->support_GL_OES_vertex_array_object){
        cxAtlasInitVAO(pview);
    }else{
        cxAtlasInitVBO(pview);
    }
}

void cxAtlasSetCapacity(cxAny pview,cxInt capacity)
{
    cxAtlas this = pview;
    CX_RETURN(this->capacity >= capacity);
    this->capacity = capacity;
    cxInt size = this->capacity * sizeof(cxBoxPoint);
    this->boxes = allocator->realloc(this->boxes,size);
    CX_ASSERT(this->boxes != NULL, "out of memory");
    size = this->capacity * 6 * sizeof(GLushort);
    this->indices = allocator->realloc(this->indices,size);
    CX_ASSERT(this->indices != NULL, "out of memory");
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



