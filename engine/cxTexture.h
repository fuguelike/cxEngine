//
//  cxTexture.h
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTexture_h
#define cxEngine_cxTexture_h

#include <cxcore/cxStream.h>
#include "cxOpenGL.h"
#include "cxShader.h"

CX_C_BEGIN

CX_DEF(cxTexCoord, cxObject)
    cxBool rotated;
    cxBool trimmed;
    cxRect4f frame;
    cxSize2f sourceSize;
    cxRect4f spriteSourceSize;
CX_END(cxTexCoord, cxObject)

cxTexCoord cxTexCoordMiss(cxConstChars key);

//fix trimmed coord to box
cxBox4f cxTexCoordTrimmedFix(cxTexCoord coord,cxBox4f vbox,cxSize2f size,cxVec2f offset,cxVec2f scale,cxBool flipx,cxBool flipy);

CX_DEF(cxTexture, cxObject)
    cxHash keys;    //cxTexCoord
    GLuint textureId;
    cxTextureParams texParam;
    cxBool isLoad;
    cxBool isSetParam;
    cxBool hasAlpha;
    cxBool hasMipmap;
    cxSize2f size;
    cxShader shader;
    cxFloat scale;
CX_END(cxTexture, cxObject)

typedef struct {
    cxTexture texture;
    cxTexCoord coord;
}cxTextureLoaderInfo;

cxTextureLoaderInfo cxTextureLoader(cxConstChars url);

void cxDrawClippingTexture(cxAny ptex,const cxVec2f pos,const cxSize2f size,cxConstChars tkey);

void cxTextureDraw(cxAny ptex,const cxVec2f pos,const cxSize2f size,cxBox4f pixel,cxConstChars tkey,cxConstChars skey);

cxBoxTex2f cxTextureBox(cxAny ptex,cxConstChars key);
//get texcoord info
cxTexCoord cxTextureCoord(cxAny ptex,cxConstChars key);
//pixel keep 
cxBoxTex2f cxTextureBoxPixel(cxAny ptex,cxConstChars key,cxBox4f pixel,cxBool flipx,cxBool flipy);

cxBoxTex2f cxTextureBoxCoord(cxAny ptex,cxTexCoord coord,cxBox4f pixel,cxBool flipx,cxBool flipy);

cxSize2f cxTextureSize(cxAny ptex,cxConstChars key,cxSize2f dv);

void cxTextureLoad(cxAny ptex,cxStream stream);

void cxTextureBind(cxAny ptex);

void cxTextureSetParam(cxAny ptex,GLuint type,GLuint value);

void cxTextureSetRepeat(cxAny ptex);

CX_C_END

#endif
