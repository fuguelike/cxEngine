//
//  cxAtlas.c
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include <core/cxEngine.h>
#include <core/cxViewXML.h>
#include "cxAtlas.h"

cxBool cxAtlasXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxAtlas this = mView;
    //support boxes mode
    cxChar *sitems = cxXMLAttr("cxAtlas.boxes");
    if(sitems != NULL){
        CX_RETAIN_SWAP(this->boxesKey, cxStringAllocChars(sitems));
    }
    xmlFree(sitems);
    //support scale9 mode
    if(cxXMLReadFloatsAttr(reader, "cxAtlas.scale9", &this->scale9.box.l) == 4){
        this->scale9.enable = true;
        cxAtlasSetNumber(this, 9);
    }
    //
    cxSpriteXMLReadAttr(xmlView, mView, reader);
    return true;
}

//on resize load
void cxAtlasLoadBoxes(cxAny pview)
{
    cxAtlas this = pview;
    CX_RETURN(this->boxesKey == NULL);
    cxTypes types = cxEngineDataSet(cxStringBody(this->boxesKey));
    CX_RETURN(types == NULL || types->type != cxTypesAtlasBoxPoint);
    
    CX_ASSERT(types->assist, "boxes assist null");
    cxTexture texture = cxTextureFactoryLoadFile(cxStringBody(types->assist));
    
    CX_ASSERT(texture != NULL, "texture load failed");
    cxSpriteSetTexture(this, texture);
    
    cxAtlasClean(this);
    cxSize2f size = cxViewSize(pview);
    CX_TYPES_FOREACH(types, cxAtlasBoxPointType, tmp){
        cxAtlasBoxPointType box = *tmp;
        cxSize2f boxsize = box.size;
        cxVec2f boxpos = box.pos;
        if((box.mask & cxViewAutoResizeLeft) && (box.mask & cxViewAutoResizeRight)){
            boxsize.w = size.w - box.box.l - box.box.r;
        }
        if(box.mask & cxViewAutoResizeLeft){
            boxpos.x = boxsize.w * 0.5f + box.box.l - size.w/2.0f;
        }
        if(box.mask & cxViewAutoResizeRight){
            boxpos.x = size.w/2.0f - boxsize.w * 0.5f - box.box.r;
        }
        if((box.mask & cxViewAutoResizeTop) && (box.mask & cxViewAutoResizeBottom)){
            boxsize.h = size.h - box.box.t - box.box.b;
        }
        if(box.mask & cxViewAutoResizeTop){
            boxpos.y = size.h/2.0f - boxsize.h * 0.5f - box.box.t;
        }
        if(box.mask & cxViewAutoResizeBottom){
            boxpos.y = boxsize.h * 0.5f -size.h/2.0f + box.box.b;
        }
        cxBoxPoint bp = cxAtlasCreateBoxPoint(boxpos, boxsize, box.texbox, box.color);
        cxAtlasAppend(this, bp);
    }
}

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
    cxAtlasLoadBoxes(event->sender);
}

CX_OBJECT_INIT(cxAtlas, cxSprite)
{
    this->isDirty = true;
    glGenVertexArrays(1, &this->vaoid);
    glGenBuffers(2, this->vboid);
    CX_METHOD_SET(this->super.super.Draw, cxAtlasDraw);
    cxObjectSetXMLReadFunc(this, cxAtlasXMLReadAttr);
    CX_EVENT_QUICK(this->super.super.onResize, cxAtlasResize);
}
CX_OBJECT_FREE(cxAtlas, cxSprite)
{
    CX_RELEASE(this->boxesKey);
    allocator->free(this->boxes);
    allocator->free(this->indices);
    glDeleteBuffers(2, this->vboid);
    glDeleteVertexArrays(1, &this->vaoid);
}
CX_OBJECT_TERM(cxAtlas, cxSprite)

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
        cxAtlasSetNumber(pview, this->capacity + 8);
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

void cxAtlasDrawInit(cxAny pview)
{
    if(cxOpenGLInstance()->support_GL_OES_vertex_array_object){
        cxAtlasInitVAO(pview);
    }else{
        cxAtlasInitVBO(pview);
    }
}

void cxAtlasSetNumber(cxAny pview,cxInt capacity)
{
    cxAtlas this = pview;
    CX_RETURN(this->capacity >= capacity);
    this->capacity = capacity;
    
    cxInt size = this->capacity * sizeof(cxBoxPoint);
    this->boxes = allocator->realloc(this->boxes,size);
    CX_ASSERT(this->boxes != NULL, "out of memory");
    memset(this->boxes, 0, size);
    
    size = this->capacity * 6 * sizeof(GLushort);
    this->indices = allocator->realloc(this->indices,size);
    CX_ASSERT(this->indices != NULL, "out of memory");
    memset(this->indices, 0, size);
    
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



