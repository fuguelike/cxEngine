//
//  cxSprite.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <engine/cxOpenGL.h>
#include <textures/cxTextureFactory.h>
#include "cxSprite.h"

void cxSpriteSetBlendFactor(cxAny pview,GLenum sfactor, GLenum dfactor)
{
    CX_ASSERT_THIS(pview, cxSprite);
    this->sfactor = sfactor;
    this->dfactor = dfactor;
}

void cxSpriteBindTexture(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    cxOpenGLSetBlendFactor(this->sfactor, this->dfactor);
    cxShaderUsing(this->shader);
    cxTextureBind(this->texture);
}

void cxSpriteDraw(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->texture == NULL);
    cxSpriteBindTexture(pview);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    cxOpenGLVertexAttribPointer(cxVertexAttribPosition, 3, sizeof(cxVec3f), &this->vbox);
    cxOpenGLVertexAttribPointer(cxVertexAttribTexcoord, 2, sizeof(cxTex2f), &this->tbox);
    cxOpenGLVertexAttribPointer(cxVertexAttribColor,    4, sizeof(cxColor4f), &this->cbox);
    cxOpenGLDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static void cxSpriteOnDirty(cxAny sender)
{
    CX_ASSERT_THIS(sender, cxSprite);
    //set color
    cxColor4f color = cxViewGetColor(this);
    this->cbox.lb = color;
    this->cbox.rb = color;
    this->cbox.lt = color;
    this->cbox.rt = color;
    //set pos
    cxBox4f box = cxViewBox(this);
    this->vbox.lb = cxVec3fv(box.l, box.b, 0.0f);
    this->vbox.rb = cxVec3fv(box.r, box.b, 0.0f);
    this->vbox.lt = cxVec3fv(box.l, box.t, 0.0f);
    this->vbox.rt = cxVec3fv(box.r, box.t, 0.0f);
    this->tbox = this->texCoord;
    //set flipX type
    if(this->isFlipX){
        this->tbox.lb.u = this->texCoord.rb.u;
        this->tbox.rb.u = this->texCoord.lb.u;
        this->tbox.lt.u = this->texCoord.rt.u;
        this->tbox.rt.u = this->texCoord.lt.u;
    }
    //set flipY type
    if(this->isFlipY){
        this->tbox.lb.v = this->texCoord.lt.v;
        this->tbox.rb.v = this->texCoord.rt.v;
        this->tbox.lt.v = this->texCoord.lb.v;
        this->tbox.rt.v = this->texCoord.rb.v;
    }
}

void cxSpriteSetFlipX(cxAny pview,cxBool flipx)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->isFlipX == flipx);
    this->isFlipX = flipx;
    cxViewSetDirty(pview, true);
}

void cxSpriteSetFlipY(cxAny pview,cxBool flipy)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->isFlipY == flipy);
    this->isFlipY = flipy;
    cxViewSetDirty(pview, true);
}

void cxSpriteSetImage(cxAny pview,cxConstChars url)
{
    cxSpriteSetTextureURL(pview, url);
}

void cxSpriteSetTextureURL(cxAny pview,cxConstChars url)
{
    CX_RETURN(url == NULL);
    CX_ASSERT_THIS(pview, cxSprite);
    cxUrlPath path = cxUrlPathParse(url);
    CX_RETURN(path == NULL);
    cxTexture texture = cxTextureFactoryLoadFile(path->path);
    if(texture == NULL){
        CX_ERROR("set texture null");
        return;
    }
    cxSpriteSetTexture(this, texture);
    if(path->count >= 2){
        this->texCoord = cxTextureBox(this->texture, path->key);
    }
    if(texture->shader != NULL){
        CX_RETAIN_SWAP(this->shader, texture->shader);
    }
}

CX_SETTER_DEF(cxSprite, texture)
{
    cxConstChars texture = cxJsonToConstChars(value);
    CX_RETURN(texture == NULL);
    cxSpriteSetTextureURL(this, texture);
}
CX_SETTER_DEF(cxSprite, shader)
{
    cxConstChars name = cxJsonToConstChars(value);
    cxShader shader = cxOpenGLShaderByName(name);
    CX_RETAIN_SWAP(this->shader, shader);
}

CX_OBJECT_TYPE(cxSprite, cxView)
{
    CX_PROPERTY_SETTER(cxSprite, texture);
    CX_PROPERTY_SETTER(cxSprite, shader);
}
CX_OBJECT_INIT(cxSprite, cxView)
{
    this->texCoord = cxBoxTex2fDefault();
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CX_ADD(cxView, this, onDirty, cxSpriteOnDirty);
    CX_SET(cxView, this, Draw, cxSpriteDraw);
    cxSpriteSetShader(this, cxShaderDefaultKey);
}
CX_OBJECT_FREE(cxSprite, cxView)
{
    CX_RELEASE(this->shader);
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxSprite, cxView)

cxSprite cxSpriteCreateWithURL(cxConstChars url)
{
    cxSprite this = CX_CREATE(cxSprite);
    cxSpriteSetTextureURL(this, url);
    return this;
}

void cxSpriteSetBoxTex(cxAny pview,cxBoxTex2f box)
{
    CX_ASSERT_THIS(pview, cxSprite);
    this->texCoord = box;
    cxViewSetDirty(this, true);
}

cxBoxColor4f cxSpriteBoxColor(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    return this->cbox;
}

cxBoxTex2f cxSpriteBoxTex(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    return this->tbox;
}

void cxSpriteSetTextureKey(cxAny pview,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxSprite);
    if(this->texture == NULL){
        cxTexture texture = cxTextureFactoryLoadFile(key);
        cxSpriteSetTexture(pview, texture);
    }else{
        this->texCoord = cxTextureBox(this->texture, key);
    }
    CX_ASSERT(this->texture != NULL, "sprite texture not load");
    cxViewSetDirty(this, true);
}

void cxSpriteSetShader(cxAny pview,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(key == NULL);
    cxShader shader = cxOpenGLShader(key);
    CX_RETURN(shader == NULL);
    CX_RETAIN_SWAP(this->shader, shader);
}

void cxSpriteSetTexture(cxAny pview,cxTexture texture)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->texture == texture);
    CX_RETAIN_SWAP(this->texture, texture);
    cxViewSetDirty(this, true);
}


