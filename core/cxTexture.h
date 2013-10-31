//
//  cxTexture.h
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTexture_h
#define cxEngine_cxTexture_h

#include "cxBase.h"
#include "cxOpenGL.h"
#include "cxStream.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxTexCoord, cxObject)
    cxBool isRotation;
    cxFloat x;
    cxFloat y;
    cxFloat w;
    cxFloat h;
CX_OBJECT_END(cxTexCoord)

typedef struct {
    cxBool (*Load)(cxAny this,cxStream stream);
    void (*Bind)(cxAny this);
}cxTextureInterface;

CX_OBJECT_DEF(cxTexture, cxObject)
    cxHash keys;    //cxTextureElement
    GLuint textureId;
    cxTextureParams texParam;
    cxBool isLoad;
    cxBool isSetParam;
    cxBool hasAlpha;
    cxBool hasMipmap;
    cxSize2f size;
    const cxTextureInterface *interface;
CX_OBJECT_END(cxTexture)

void cxDrawClippingTexture(cxTexture this,const cxVec2f pos,const cxSize2f size,cxConstChars tkey);

void cxTextureDraw(cxTexture this,const cxVec2f pos,const cxSize2f size,cxConstChars tkey,cxConstChars skey);

cxBoxTex2f cxTextureBox(cxTexture this,cxConstChars key);

cxRect4f cxTextureRect(cxTexture this,cxConstChars key);

cxSize2f cxTextureSize(cxTexture this,cxConstChars key);

void cxTextureLoad(cxTexture this,cxStream stream);

void cxTextureBind(cxTexture this);

void cxTextureSetParam(cxTexture this,GLuint type,GLuint value);

CX_C_END

#endif
