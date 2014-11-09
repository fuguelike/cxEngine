//
//  cxSprite.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <engine/cxOpenGL.h>
#include <textures/cxTextureCache.h>
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
    cxShaderUsing(this->Shader);
    cxTextureFireBind(this->Texture);
}

static void cxSpriteOnDirty(cxAny sender)
{
    CX_ASSERT_THIS(sender, cxSprite);
    cxViewDirty dirty = cxViewGetDirty(this);
    //set color
    if(dirty & cxViewDirtyColor){
        cxColor4f color = cxViewGetColor(this);
        this->cbox.lb = color;
        this->cbox.rb = color;
        this->cbox.lt = color;
        this->cbox.rt = color;
    }
    //set pos
    if(dirty & cxViewDirtySize){
        cxBox4f box = cxViewGetBox(this);
        this->vbox.lb = cxVec3fv(box.l, box.b, 0.0f);
        this->vbox.rb = cxVec3fv(box.r, box.b, 0.0f);
        this->vbox.lt = cxVec3fv(box.l, box.t, 0.0f);
        this->vbox.rt = cxVec3fv(box.r, box.t, 0.0f);
    }
    //set texture coord
    if(dirty & cxViewDirtyTexture){
        this->tbox = cxBoxTex2fFlip(this->texCoord, this->isFlipX, this->isFlipY);
    }
}

void cxSpriteSetFlip(cxAny pview,cxBool flipx,cxBool flipy)
{
    cxSpriteSetFlipX(pview, flipx);
    cxSpriteSetFlipY(pview, flipy);
}

void cxSpriteSetFlipX(cxAny pview,cxBool flipx)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->isFlipX == flipx);
    this->isFlipX = flipx;
    cxViewSetDirty(pview, cxViewDirtyTexture);
}

void cxSpriteSetFlipY(cxAny pview,cxBool flipy)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->isFlipY == flipy);
    this->isFlipY = flipy;
    cxViewSetDirty(pview, cxViewDirtyTexture);
}

void cxSpriteSetTextureURL(cxAny pview,cxConstChars url)
{
    CX_ASSERT_THIS(pview, cxSprite);
    cxTextureLoaderInfo info = cxTextureLoader(url);
    CX_RETURN(info.texture == NULL);
    if(info.texture != NULL){
        cxSpriteSetTexture(this, info.texture);
    }
    if(info.hasCoord){
        this->tbox = this->texCoord = info.coord;
    }
    if(info.texture != NULL && info.texture->shader != NULL){
        CX_RETAIN_SWAP(this->Shader, info.texture->shader);
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
    CX_RETAIN_SWAP(this->Shader, shader);
}
CX_SETTER_DEF(cxSprite, blend)
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
CX_METHOD_DEF(cxSprite,Draw,void)
{
    CX_RETURN(this->Texture == NULL);
    cxSpriteBindTexture(this);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    cxOpenGLVertexAttribPointer(cxVertexAttribPosition, 3, sizeof(cxVec3f), &this->vbox);
    cxOpenGLVertexAttribPointer(cxVertexAttribTexcoord, 2, sizeof(cxTex2f), &this->tbox);
    cxOpenGLVertexAttribPointer(cxVertexAttribColor,    4, sizeof(cxColor4f), &this->cbox);
    cxOpenGLDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
CX_TYPE(cxSprite, cxView)
{
    CX_SETTER(cxSprite, blend);
    CX_SETTER(cxSprite, texture);
    CX_SETTER(cxSprite, shader);
    CX_METHOD(cxSprite, Draw);
}
CX_INIT(cxSprite, cxView)
{
    this->tbox = this->texCoord = cxBoxTex2fDefault();
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CX_ADD(cxView, this, onDirty, cxSpriteOnDirty);
    cxSpriteSetShader(this, cxShaderDefaultKey);
}
CX_FREE(cxSprite, cxView)
{
    CX_RELEASE(this->Shader);
    CX_RELEASE(this->Texture);
}
CX_TERM(cxSprite, cxView)

cxSprite cxSpriteCreateWithURL(cxConstChars url)
{
    cxSprite this = CX_CREATE(cxSprite);
    cxSpriteSetTextureURL(this, url);
    return this;
}

void cxSpriteSetBoxTex(cxAny pview,cxBoxTex2f box)
{
    CX_ASSERT_THIS(pview, cxSprite);
    this->tbox = this->texCoord = box;
    cxViewSetDirty(pview, cxViewDirtyTexture);
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
    if(this->Texture == NULL){
        cxTextureLoaderInfo info = cxTextureLoader(key);
        cxSpriteSetTexture(pview, info.texture);
    }else{
        this->tbox = this->texCoord = cxTextureBox(this->Texture, key);
    }
    CX_ASSERT(this->Texture != NULL, "sprite texture not load");
    cxViewSetDirty(pview, cxViewDirtyTexture);
}

void cxSpriteSetShader(cxAny pview,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(key == NULL);
    cxShader shader = cxOpenGLShader(key);
    CX_RETURN(shader == NULL);
    CX_RETAIN_SWAP(this->Shader, shader);
}

void cxSpriteSetTexture(cxAny pview,cxTexture texture)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->Texture == texture);
    CX_RETAIN_SWAP(this->Texture, texture);
    cxViewSetDirty(pview, cxViewDirtyTexture);
}


