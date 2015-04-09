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

cxBool cxSpriteBindTexture(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    cxOpenGLSetBlendFactor(this->sfactor, this->dfactor);
    cxShader shader = cxViewGetShader(this);
    if(shader == NULL){
        return false;
    }
    cxShaderUsing(shader,this);
    if(this->Texture != NULL){
        cxTextureBind(this->Texture);
    }
    return true;
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

void cxSpriteSetRepeatTexture(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    cxTexture texture = cxSpriteGetTexture(this);
    if(texture == NULL){
        return;
    }
    cxTextureSetRepeat(texture);
}

void cxSpriteSetTextureURL(cxAny pview,cxConstChars url)
{
    CX_ASSERT_THIS(pview, cxSprite);
    cxTextureLoaderInfo info = cxTextureLoader(url);
    CX_RETURN(info.texture == NULL);
    cxSpriteSetTexture(this, info.texture);
    if(info.coord != NULL){
        cxSpriteSetCoord(this, info.coord);
    }
    if(info.texture->shader != NULL){
        cxViewSetShader(this, info.texture->shader);
    }
}

void cxSpriteUseTextureSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    cxTexCoord coord = cxSpriteGetCoord(this);
    if(coord == NULL){
        return;
    }
    cxViewSetSize(this, coord->sourceSize);
}

cxSprite cxSpriteCreateWithTexture(cxTexture texture)
{
    cxSprite this = CX_CREATE(cxSprite);
    cxSpriteSetTexture(this, texture);
    return this;
}

//support texturepacker trimmed
static void cxSpriteComputeBox(cxSprite this)
{
    cxBox4f box = cxViewGetBox(this);
    cxSize2f size = cxViewGetSize(this);
    box = cxTexCoordTrimmedFix(this->Coord, box,size,cxVec2fv(0, 0),cxVec2fv(1.0, 1.0),this->isFlipX,this->isFlipY);
    cxFloat z = cxViewGetZ(this);
    this->vbox.lb = cxVec3fv(box.l, box.b, z);
    this->vbox.rb = cxVec3fv(box.r, box.b, z);
    this->vbox.lt = cxVec3fv(box.l, box.t, z);
    this->vbox.rt = cxVec3fv(box.r, box.t, z);
}

static void cxSpriteComputeColor(cxSprite this)
{
    cxColor4f color = cxViewGetColor(this);
    this->cbox.lb = color;
    this->cbox.rb = color;
    this->cbox.lt = color;
    this->cbox.rt = color;
}

CX_SETTER_DEF(cxSprite, texture)
{
    cxConstChars texture = cxJsonToConstChars(value);
    CX_RETURN(texture == NULL);
    cxSpriteSetTextureURL(this, texture);
}
CX_SETTER_DEF(cxSprite, pixel)
{
    this->Pixel = cxJsonToBox4f(value, this->Pixel);
}
CX_SETTER_DEF(cxSprite, blend)
{
    cxConstChars blend = cxJsonToConstChars(value);
    if(cxConstCharsEqu(blend, "add")){
        cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE);
    }else if(cxConstCharsEqu(blend, "multiply")){
        cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}
CX_METHOD_DEF(cxSprite,OnDraw,void)
{
    cxSpriteDraw(this);
}
CX_METHOD_DEF(cxSprite, OnDirty, void)
{
    cxViewDirty dirty = cxViewGetDirty(this);
    //set color
    if(dirty & cxViewDirtyColor){
        cxSpriteComputeColor(this);
    }
    //set texture coord
    if(this->Texture != NULL && (dirty & cxViewDirtyTexture)){
        this->tbox = cxTextureBoxCoord(this->Texture, this->Coord, this->Pixel, this->isFlipX, this->isFlipY);
    }
    //set position
    if((dirty & cxViewDirtyTexture) || (dirty & cxViewDirtySize)){
        cxSpriteComputeBox(this);
    }
}
CX_TYPE(cxSprite, cxView)
{
    CX_SETTER(cxSprite, blend);
    CX_SETTER(cxSprite, texture);
    CX_SETTER(cxSprite, pixel);
    
    CX_METHOD(cxSprite, OnDraw);
    CX_METHOD(cxSprite, OnDirty);
}
CX_INIT(cxSprite, cxView)
{
    this->tbox = cxBoxTex2fDefault;
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxViewSetShader(this, cxOpenGLShader(cxShaderDefaultKey));
}
CX_FREE(cxSprite, cxView)
{
    CX_RELEASE(this->Coord);
    CX_RELEASE(this->Texture);
}
CX_TERM(cxSprite, cxView)

void cxSpriteDraw(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    if(!cxSpriteBindTexture(this)){
        return;
    }
    const cxVec3f *vs = &this->vbox.lt;
    const cxColor4f *cs = &this->cbox.lt;
    const cxTex2f *ts = this->Texture == NULL ? &cxBoxTex2fDefault.lt : &this->tbox.lt;
    OpenGLDrawTriangle(GL_TRIANGLE_STRIP, 4, vs, cs, ts);
}

cxSprite cxSpriteCreateWithURL(cxConstChars url)
{
    cxSprite this = CX_CREATE(cxSprite);
    cxSpriteSetTextureURL(this, url);
    return this;
}

cxSize2f cxSpriteTextureSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    return (this->Coord != NULL)?this->Coord->sourceSize:cxSize2fv(0, 0);
}

void cxSpriteSetCoord(cxAny pview,cxTexCoord coord)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->Coord == coord);
    CX_RETAIN_SWAP(this->Coord, coord);
    this->tbox = cxTextureBoxCoord(this->Texture, this->Coord, this->Pixel, this->isFlipX, this->isFlipY);
    cxViewSetDirty(this, cxViewDirtyTexture);
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

cxBoxTex2f cxSpriteBoxTexWithKey(cxAny pview,cxConstChars key,cxBool flipx,cxBool flipy)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_ASSERT(this->Texture != NULL, "texture not set");
    return cxTextureBoxPixel(this->Texture, key, this->Pixel, flipx, flipy);
}

void cxSpriteSetTextureKey(cxAny pview,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_ASSERT(this->Texture != NULL, "texture not set");
    cxTexCoord coord = cxTextureCoord(this->Texture, key);
    cxSpriteSetCoord(this, coord);
}

void cxSpriteSetTexture(cxAny pview,cxTexture texture)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_RETURN(this->Texture == texture);
    CX_RETAIN_SWAP(this->Texture, texture);
    cxViewSetDirty(pview, cxViewDirtyTexture);
}


