//
//  cxTexture.h
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTexture_h
#define cxEngine_cxTexture_h

#include "cxOpenGL.h"
#include "cxStream.h"
#include "cxShader.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxTexCoord, cxObject)
    cxBool isRotation;
    cxFloat x;
    cxFloat y;
    cxFloat w;
    cxFloat h;
CX_OBJECT_END(cxTexCoord, cxObject)

CX_OBJECT_DEF(cxTexture, cxObject)
    cxHash keys;    //cxTexCoord
    GLuint textureId;
    cxTextureParams texParam;
    cxBool isLoad;
    cxBool isSetParam;
    cxBool hasAlpha;
    cxBool hasMipmap;
    cxSize2f size;
    cxVec2f scale;
    CX_METHOD_DEF(cxBool, Load, cxAny, cxStream);
    CX_METHOD_DEF(void, Bind, cxAny);
    cxShader shader;
CX_OBJECT_END(cxTexture, cxObject)

void cxDrawClippingTexture(cxAny ptex,const cxVec2f pos,const cxSize2f size,cxConstChars tkey);

void cxTextureDraw(cxAny ptex,const cxVec2f pos,const cxSize2f size,cxConstChars tkey,cxConstChars skey);

cxBoxTex2f cxTextureBox(cxAny ptex,cxConstChars key);

cxBoxTex2f cxTextureBoxPixel(cxAny ptex,cxConstChars key,cxFloat subPixel);

cxRect4f cxTextureRect(cxAny ptex,cxConstChars key);

cxSize2f cxTextureSize(cxAny ptex,cxConstChars key);

void cxTextureLoad(cxAny ptex,cxStream stream);

void cxTextureBind(cxAny ptex);

void cxTextureSetParam(cxAny ptex,GLuint type,GLuint value);

CX_C_END

#endif
