//
//  cxSprite.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxOpenGL.h>
#include <core/cxXMLScript.h>
#include <textures/cxTextureFactory.h>
#include "cxSprite.h"
#include "cxViewXML.h"

void cxSpriteSetBlendFactor(cxAny pview,GLenum sfactor, GLenum dfactor)
{
    cxSprite this = pview;
    this->sfactor = sfactor;
    this->dfactor = dfactor;
}

void cxSpriteDraw(cxAny pview)
{
    cxSprite this = pview;
    CX_RETURN(this->texture == NULL);
    cxOpenGLSetBlendFactor(this->sfactor, this->dfactor);
    cxShaderUsing(this->shader);
    cxTextureBind(this->texture);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxVec3f), &this->vbox);
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxTexCoord2f), &this->tbox);
    glVertexAttribPointer(cxVertexAttribColor,    4, GL_FLOAT, GL_FALSE, sizeof(cxColor4f), &this->cbox);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void cxSpriteDirtyEvent(cxEvent *event)
{
    cxSprite this = event->sender;
    this->cbox.lb = this->super.color;
    this->cbox.rb = this->super.color;
    this->cbox.lt = this->super.color;
    this->cbox.rt = this->super.color;
    
    cxBoxVec2f box = cxViewLocationBox(this);
    this->vbox.lb = cxVec3fv(box.lb.x, box.lb.y, 0.0f);
    this->vbox.rb = cxVec3fv(box.rb.x, box.rb.y, 0.0f);
    this->vbox.lt = cxVec3fv(box.lt.x, box.lt.y, 0.0f);
    this->vbox.rt = cxVec3fv(box.rt.x, box.rt.y, 0.0f);
    
    this->tbox = this->texCoord;
    if(this->isFlipX) {
        this->tbox.lb.u = this->texCoord.rb.u;
        this->tbox.rb.u = this->texCoord.lb.u;
        this->tbox.lt.u = this->texCoord.rt.u;
        this->tbox.rt.u = this->texCoord.lt.u;
    }
    if(this->isFlipY) {
        this->tbox.lb.v = this->texCoord.lt.v;
        this->tbox.rb.v = this->texCoord.rt.v;
        this->tbox.lt.v = this->texCoord.lb.v;
        this->tbox.rt.v = this->texCoord.rb.v;
    }
}

void cxSpriteSetFlipX(cxAny pview,cxBool flipx)
{
    cxSprite this = pview;
    this->isFlipX = flipx;
    cxViewSetDirty(pview, true);
}

void cxSpriteSetFlipY(cxAny pview,cxBool flipy)
{
    cxSprite this = pview;
    this->isFlipY = flipy;
    cxViewSetDirty(pview, true);
}

//texture="res/a.xml?green.png"
cxBool cxSpriteXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    //invoke base
    cxViewXMLReadAttr(xmlView, mView, reader);
    
    cxSprite this = mView;
    //texture
    cxChar *surl = cxXMLAttr("cxSprite.texture");
    CX_RETURN(surl == NULL, true);
    cxChar file[128];
    cxChar key[64];
    cxInt rv = cxParseURL(surl, file, key);
    CX_RETURN(rv <= 0, true);
    cxTexture texture = cxTextureFactoryLoadFile(file);
    CX_ASSERT(texture != NULL, "texture load failed %s",file);
    cxSpriteSetTexture(this, texture);
    //use texture size
    cxBool useTexSize = cxViewZeroSize(this);
    if(useTexSize){
        cxViewSetSize(this, texture->size);
    }
    if(rv > 1){
        cxSpriteSetTextureKey(this, key, useTexSize);
    }
    xmlFree(surl);
    //flipx flipy
    cxSpriteSetFlipX(this, cxXMLReadBoolAttr(reader, "cxSprite.flipX", this->isFlipX));
    cxSpriteSetFlipY(this, cxXMLReadBoolAttr(reader, "cxSprite.flipY", this->isFlipY));
    //shader
    cxChar *shader = cxXMLAttr("cxSprite.shader");
    if(shader != NULL){
        cxSpriteSetShader(this, shader);
    }
    xmlFree(shader);
    return true;
}

CX_OBJECT_INIT(cxSprite, cxView)
{
    this->texCoord = cxBoxTex2fDefault();
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CX_EVENT_QUICK(this->super.onDirty, cxSpriteDirtyEvent);
    CX_METHOD_SET(this->super.Draw, cxSpriteDraw);
    cxSpriteSetShader(this, cxShaderDefaultKey);
    cxObjectSetXMLReadFunc(this, cxSpriteXMLReadAttr);
}
CX_OBJECT_FREE(cxSprite, cxView)
{
    CX_RELEASE(this->shader);
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxSprite, cxView)

cxSprite cxSpriteCreateWithXML(cxConstChars xml,cxConstChars key)
{
    cxSprite this = CX_CREATE(cxSprite);
    cxTexture texture = cxTextureFactoryLoadFile(xml);
    cxSpriteSetTexture(this, texture);
    cxSpriteSetTextureKey(this, key, true);
    return this;
}

cxBoxTex2f cxSpriteBoxTex(cxAny pview)
{
    cxSprite this = pview;
    return this->texCoord;
}

void cxSpriteSetTextureKey(cxAny pview,cxConstChars key,cxBool equSize)
{
    cxSprite this = pview;
    if(this->texture == NULL){
        cxTexture texture = cxTextureFactoryLoadFile(key);
        cxSpriteSetTexture(pview, texture);
    }else{
        this->texCoord = cxTextureBox(this->texture, key);
    }
    CX_ASSERT(this->texture != NULL, "sprite texture not load");
    if(equSize){
        cxViewSetSize(pview, cxTextureSize(this->texture, key));
    }
}

void cxSpriteSetShader(cxAny pview,cxConstChars key)
{
    CX_RETURN(key == NULL);
    cxShader shader = cxOpenGLShader(key);
    CX_RETURN(shader == NULL);
    cxSprite this = pview;
    CX_RETAIN_SWAP(this->shader, shader);
}

void cxSpriteSetTexture(cxAny pview,cxTexture texture)
{
    cxSprite this = pview;
    CX_RETAIN_SWAP(this->texture, texture);
}


