//
//  cxTexture.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include "cxShader.h"
#include "cxTexture.h"

CX_TYPE(cxTexCoord, cxObject)
{}
CX_INIT(cxTexCoord, cxObject)
{}
CX_FREE(cxTexCoord, cxObject)
{}
CX_TERM(cxTexCoord, cxObject)

CX_TYPE(cxTexture, cxObject)
{}
CX_INIT(cxTexture, cxObject)
{
    this->texParam = cxtpv(GL_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
    this->keys = CX_ALLOC(cxHash);
    this->isSetParam = true;
}
CX_FREE(cxTexture, cxObject)
{
    cxOpenGLDeleteTexture(this->textureId, 0);
    CX_RELEASE(this->keys);
    CX_RELEASE(this->shader);
}
CX_TERM(cxTexture, cxObject)

void cxTextureLoad(cxAny ptex,cxStream stream)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    CX_RETURN(this->isLoad);
    this->isLoad = CX_METHOD_GET(false, this->Load,this,stream);
}

cxBoxTex2f cxTextureBoxPixel(cxAny ptex,cxConstChars key,cxFloat pixel,cxBool flipx,cxBool flipy)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    CX_ASSERT(this != NULL, "texture null");
    cxBoxTex2f rv = cxBoxTex2fDefault();
    CX_RETURN(key == NULL, rv);
    cxTexCoord coord = cxHashGet(this->keys, cxHashStrKey(key));
    if(coord == NULL){
        return rv;
    }
    cxFloat x = (coord->x + pixel) / this->size.w;
    cxFloat y = (coord->y + pixel) / this->size.h;
    cxFloat w = (coord->w - pixel*2.0f) / this->size.w;
    cxFloat h = (coord->h - pixel*2.0f) / this->size.h;
    rv.lb = cxTex2fv(x, y + h);
    rv.rb = cxTex2fv(x + w, y + h);
    rv.lt = cxTex2fv(x, y);
    rv.rt = cxTex2fv(x + w, y);
    return cxBoxTex2fFlip(rv, flipx, flipy);
}

cxBoxTex2f cxTextureBox(cxAny ptex,cxConstChars key)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    return cxTextureBoxPixel(this, key, 0, false, false);
}

cxSize2f cxTextureSize(cxAny ptex,cxConstChars key)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    CX_ASSERT(cxConstCharsOK(key), "key error");
    cxTexCoord coord = cxHashGet(this->keys, cxHashStrKey(key));
    if(coord == NULL){
        return this->size;
    }
    return cxSize2fv(coord->w, coord->h);
}

void cxDrawClippingTexture(cxAny ptex,const cxVec2f pos,const cxSize2f size,cxConstChars tkey)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxTextureDraw(this, pos, size, tkey, cxShaderClippingKey);
}

void cxTextureDraw(cxAny ptex,const cxVec2f pos,const cxSize2f size,cxConstChars tkey,cxConstChars skey)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxBoxVec3f vbox;
    cxFloat wh = size.w / 2.0f;
    cxFloat hh = size.h / 2.0f;
    vbox.lb = cxVec3fv(pos.x - wh, pos.y - hh, 0);
    vbox.lt = cxVec3fv(pos.x - wh, pos.y + hh, 0);
    vbox.rb = cxVec3fv(pos.x + wh, pos.y - hh, 0);
    vbox.rt = cxVec3fv(pos.x + wh, pos.y + hh, 0);
    cxBoxTex2f tbox = cxBoxTex2fDefault();
    if(tkey != NULL){
        tbox = cxTextureBox(this, tkey);
    }
    cxTextureBind(this);
    cxOpenGLSetBlendFactor(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxOpenGLUsingShader(skey);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    cxOpenGLVertexAttribPointer(cxVertexAttribPosition, 3, sizeof(cxVec3f), &vbox);
    cxOpenGLVertexAttribPointer(cxVertexAttribTexcoord, 2, sizeof(cxTex2f), &tbox);
    cxOpenGLVertexAttribPointer(cxVertexAttribColor,    4, sizeof(cxColor4f), &cxBoxColor4fDefault);
    cxOpenGLDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void cxTextureBind(cxAny ptex)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    CX_METHOD_RUN(this->Bind,this);
    if(!this->isSetParam){
        cxOpenGLSetTexParameters(this->texParam);
        this->isSetParam = true;
    }
}

void cxTextureSetParam(cxAny ptex,GLuint type,GLuint value)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    if(type == GL_TEXTURE_MIN_FILTER && this->texParam.minFilter != value){
        this->texParam.minFilter = value;
        this->isSetParam = false;
    }
    if(type == GL_TEXTURE_MAG_FILTER && this->texParam.magFilter != value){
        this->texParam.magFilter = value;
        this->isSetParam = false;
    }
    if(type == GL_TEXTURE_WRAP_S && this->texParam.wrapS != value){
        this->texParam.wrapS = value;
        this->isSetParam = false;
    }
    if(type == GL_TEXTURE_WRAP_T && this->texParam.wrapT != value){
        this->texParam.wrapT = value;
        this->isSetParam = false;
    }
}




