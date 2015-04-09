//
//  cxSprite.h
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSprite_h
#define cxEngine_cxSprite_h

#include <engine/cxView.h>
#include <engine/cxShader.h>
#include <engine/cxTexture.h>

CX_C_BEGIN

CX_DEF(cxSprite, cxView)
    CX_FIELD_DEF(cxTexture Texture);
    CX_FIELD_DEF(cxBox4f Pixel);
    CX_FIELD_DEF(cxTexCoord Coord);
    GLenum          sfactor;
    GLenum          dfactor;
    cxBoxVec3f      vbox;
    cxBoxColor4f    cbox;
    cxBoxTex2f      tbox;
    cxBool          isFlipX;
    cxBool          isFlipY;
CX_END(cxSprite, cxView)

CX_FIELD_GET(cxSprite, cxTexCoord, Coord);
CX_INLINE cxBoxTex2f *cxSpriteGetTexCoord(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxSprite);
    return &this->tbox;
}
CX_FIELD_GET(cxSprite, cxTexture, Texture);
CX_INLINE void cxSpriteSetPixel(cxAny pthis,cxBox4f pixel)
{
    CX_ASSERT_THIS(pthis, cxSprite);
    CX_RETURN(cxBox4fEqu(this->Pixel, pixel));
    this->Pixel = pixel;
    cxViewSetDirty(this, cxViewDirtyTexture);
}

void cxSpriteDraw(cxAny pview);

void cxSpriteSetBoxTex(cxAny pview,cxBoxTex2f box);

cxBoxTex2f cxSpriteBoxTex(cxAny pview);

cxBoxTex2f cxSpriteBoxTexWithKey(cxAny pview,cxConstChars key,cxBool flipx,cxBool flipy);

cxBoxColor4f cxSpriteBoxColor(cxAny pview);

cxSprite cxSpriteCreateWithURL(cxConstChars url);

cxSprite cxSpriteCreateWithTexture(cxTexture texture);

void cxSpriteSetBlendFactor(cxAny pview,GLenum sfactor, GLenum dfactor);

void cxSpriteSetFlip(cxAny pview,cxBool flipx,cxBool flipy);

void cxSpriteSetFlipX(cxAny pview,cxBool flipx);

void cxSpriteSetFlipY(cxAny pview,cxBool flipy);

cxBool cxSpriteBindTexture(cxAny pview);

void cxSpriteSetRepeatTexture(cxAny pview);

void cxSpriteSetTextureURL(cxAny pview,cxConstChars url);

void cxSpriteUseTextureSize(cxAny pview);

void cxSpriteSetTexture(cxAny pview,cxTexture texture);

void cxSpriteSetTextureKey(cxAny pview,cxConstChars key);

void cxSpriteSetCoord(cxAny pview,cxTexCoord coord);

cxSize2f cxSpriteTextureSize(cxAny pview);

CX_C_END

#endif

