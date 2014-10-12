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

CX_OBJECT_DEF(cxSprite, cxView)
    CX_FIELD_DEF(cxTexture Texture);
    CX_FIELD_DEF(cxShader Shader);
    GLenum          sfactor;
    GLenum          dfactor;
    cxBoxVec3f      vbox;
    cxBoxColor4f    cbox;
    cxBoxTex2f      tbox;
    cxBool          isFlipX;
    cxBool          isFlipY;
    cxBoxTex2f      texCoord;
CX_OBJECT_END(cxSprite, cxView)

CX_FIELD_GET(cxSprite, cxTexture, Texture);
CX_FIELD_GET(cxSprite, cxShader, Shader);

void cxSpriteSetBoxTex(cxAny pview,cxBoxTex2f box);

cxBoxTex2f cxSpriteBoxTex(cxAny pview);

cxBoxColor4f cxSpriteBoxColor(cxAny pview);

cxSprite cxSpriteCreateWithURL(cxConstChars url);

void cxSpriteSetBlendFactor(cxAny pview,GLenum sfactor, GLenum dfactor);

void cxSpriteSetFlip(cxAny pview,cxBool flipx,cxBool flipy);

void cxSpriteSetFlipX(cxAny pview,cxBool flipx);

void cxSpriteSetFlipY(cxAny pview,cxBool flipy);

void cxSpriteBindTexture(cxAny pview);

void cxSpriteDraw(cxAny pview);

void cxSpriteSetShader(cxAny pview,cxConstChars key);

void cxSpriteSetImage(cxAny pview,cxConstChars url);

void cxSpriteSetTextureURL(cxAny pview,cxConstChars url);

void cxSpriteSetTexture(cxAny pview,cxTexture texture);

void cxSpriteSetTextureKey(cxAny pview,cxConstChars key);

CX_C_END

#endif

