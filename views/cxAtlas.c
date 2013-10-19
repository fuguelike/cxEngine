//
//  cxAtlas.c
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxAtlas.h"

cxBoxPoint cxAtlasXMLReadBoxPoint(cxAny pview,xmlTextReaderPtr reader)
{
    cxSprite this = pview;
    //pos
    cxVec2f pos = cxVec2fv(0, 0);
    pos.x = cxXMLReadFloatAttr(reader, "x", pos.x);
    pos.y = cxXMLReadFloatAttr(reader, "y", pos.y);
    //size
    cxSize2f size = cxSize2fv(0, 0);
    size.w = cxXMLReadFloatAttr(reader, "w", size.w);
    size.h = cxXMLReadFloatAttr(reader, "h", size.h);
    //texbox
    cxBoxTex2f texbox = cxBoxTex2fDefault();
    xmlChar *tkey = cxXMLAttr("texture");
    if(tkey != NULL && this->texture != NULL){
        texbox = cxTextureBox(this->texture, (cxConstChars)tkey);
    }
    xmlFree(tkey);
    //color
    cxColor4f color = cxXMLReadColorAttr(reader, "color", this->super.color);
    return cxAtlasCreateBoxPoint(pos, size, texbox, color);
}

void cxAtlasXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxSpriteXMLReadAttr(xmlView, mView, reader);
    cxAtlas this = mView;
    CX_RETURN(this->super.texture == NULL);
    //read atlas attr
    
    //support scale9
    if(cxXMLReadFloatsAttr(reader, "cxAtlas.scale9", &this->scale9.box.l) == 4){
        this->scale9.enable = true;
        cxAtlasUpdateScale9(this);
        return;
    }
    //read sub element
    CX_RETURN(xmlTextReaderIsEmptyElement(reader));
    //<cxAtlasPoint x="0" y="0" w="50" h="50" texture="red.png" color="1,1,1,1" />
    while(xmlTextReaderRead(reader)){
        int type = xmlTextReaderNodeType(reader);
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(ELEMENT_END_TYPE(cxAtlas)){
            break;
        }
        if(ELEMENT_BEGIN_TYPE(cxAtlasBox)){
            cxBoxPoint bp = cxAtlasXMLReadBoxPoint(this,reader);
            cxAtlasAppend(this, bp);
        }
    }
    //
}

static void cxAtlasVAODraw(void *pview)
{
    cxAtlas this = pview;
    if(!this->isInit){
        this->isInit = true;
        cxAtlasDrawInit(pview);
    }
    if (this->isDirty){
        glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cxBoxPoint) * this->number, this->boxs);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        this->isDirty = false;
    }
    glBindVertexArray(this->vaoid);
    glDrawElements(GL_TRIANGLES, this->number*6, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}

void cxAtlasDraw(void *pview)
{
    cxAtlas this = pview;
    CX_RETURN(this->number == 0 || this->super.texture == NULL);
    cxOpenGLSetBlendFactor(this->super.sfactor, this->super.dfactor);
    cxShaderUsing(this->super.shader);
    cxTextureBind(this->super.texture);
    if(cxOpenGLInstance()->support_GL_OES_vertex_array_object){
        cxAtlasVAODraw(pview);
    }else{
        //
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
    cxAtlasSetNumber(pview, 9);
    this->number = 0;
    
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

void cxAtlasResize(cxAny pview,cxAny arg)
{
    cxAtlasUpdateScale9(pview);
}

CX_OBJECT_INIT(cxAtlas, cxSprite)
{
    this->isDirty = true;
    CX_METHOD_SET(this->super.super.Draw, cxAtlasDraw);
    cxObjectSetXMLReadFunc(this, cxAtlasXMLReadAttr);
    CX_EVENT_APPEND(this->super.super.onResize, cxAtlasResize, NULL);
}
CX_OBJECT_FREE(cxAtlas, cxSprite)
{
    allocator->free(this->boxs);
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
    if(this->boxNumber <= this->number){
        cxAtlasSetNumber(pview, this->boxNumber + 8);
    }
    CX_ASSERT(this->number < this->boxNumber, "atlas number > boxNumber");
    this->boxs[this->number++] = point;
    this->isDirty = true;
}

void cxAtlasUpdate(cxAny pview,cxInt index, cxBoxPoint point)
{
    cxAtlas this = pview;
    CX_ASSERT(index >= 0 && index < this->boxNumber, "index > boxNumber");
    this->boxs[index] = point;
    this->isDirty = true;
}

static void cxAtlasInitVAO(cxAny pview)
{
    cxAtlas this = pview;
    
    glGenVertexArrays(1, &this->vaoid);
    glBindVertexArray(this->vaoid);
    
    glGenBuffers(2, this->vboid);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboid[0]);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(cxBoxPoint) * this->boxNumber, this->boxs, GL_DYNAMIC_DRAW);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*this->boxNumber*6, this->indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cxAtlasDrawInit(cxAny pview)
{
    if(cxOpenGLInstance()->support_GL_OES_vertex_array_object){
        cxAtlasInitVAO(pview);
    }else{
        //
    }
}

void cxAtlasSetNumber(cxAny pview,cxInt boxNumber)
{
    cxAtlas this = pview;
    CX_RETURN(this->boxNumber >= boxNumber);
    this->boxNumber = boxNumber;
    
    cxInt size = this->boxNumber * sizeof(cxBoxPoint);
    this->boxs = allocator->realloc(this->boxs,size);
    CX_ASSERT(this->boxs != NULL, "out of memory");
    memset(this->boxs, 0, size);
    
    size = this->boxNumber * 6 * sizeof(GLushort);
    this->indices = allocator->realloc(this->indices,size);
    CX_ASSERT(this->indices != NULL, "out of memory");
    memset(this->indices, 0, size);
    
    for(int i=0; i < this->boxNumber;i++){
        this->indices[i*6+0] = i*4+0;
        this->indices[i*6+1] = i*4+1;
        this->indices[i*6+2] = i*4+2;
        this->indices[i*6+3] = i*4+3;
        this->indices[i*6+4] = i*4+2;
        this->indices[i*6+5] = i*4+1;
    }
}



