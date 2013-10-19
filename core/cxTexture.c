//
//  cxTexture.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTexture.h"

CX_OBJECT_INIT(cxTexCoord, cxObject)
{
    
}
CX_OBJECT_FREE(cxTexCoord, cxObject)
{
    
}
CX_OBJECT_TERM(cxTexCoord, cxObject)

CX_OBJECT_INIT(cxTexture, cxObject)
{
    this->texParam = cxtpv(GL_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
    this->keys = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxTexture, cxObject)
{
    cxOpenGLDeleteTexture(0, this->textureId);
    CX_RELEASE(this->keys);
}
CX_OBJECT_TERM(cxTexture, cxObject)

void cxTextureLoad(cxTexture this,cxStream stream)
{
    CX_RETURN(this->isLoad);
    CX_ASSERT(this->interface != NULL, "not imp texture");
    this->isLoad = this->interface->Load(this,stream);
    CX_ASSERT(this->isLoad, "texture  can not load");
}

cxBoxTex2f cxTextureBox(cxTexture this,cxConstChars key)
{
    CX_ASSERT(this != NULL, "texture null");
    cxBoxTex2f rv = cxBoxTex2fDefault();
    cxTexCoord texCoord = cxHashGet(this->keys, cxHashStrKey(key));
    if(texCoord == NULL){
        return rv;
    }
    cxFloat x = texCoord->x / this->size.w;
    cxFloat y = texCoord->y / this->size.h;
    cxFloat w = texCoord->w / this->size.w;
    cxFloat h = texCoord->h / this->size.h;
    rv.lb = cxTex2fv(x, y + h);
    rv.rb = cxTex2fv(x + w, y + h);
    rv.lt = cxTex2fv(x, y);
    rv.rt = cxTex2fv(x + w, y);
    return rv;
}

cxRect4f cxTextureRect(cxTexture this,cxConstChars key)
{
    CX_ASSERT(this != NULL, "texture null");
    cxRect4f rv = cxRect4fv(0.0f, 0.0f, 1.0f, 1.0f);
    cxTexCoord texCoord = cxHashGet(this->keys, cxHashStrKey(key));
    if(texCoord == NULL){
        return rv;
    }
    rv.x = texCoord->x / this->size.w;
    rv.y = texCoord->y / this->size.h;
    rv.w = texCoord->w / this->size.w;
    rv.h = texCoord->h / this->size.h;
    return rv;
}

cxSize2f cxTextureSize(cxTexture this,cxConstChars key)
{
    CX_ASSERT(this != NULL, "texture null");
    cxSize2f rv = this->size;
    cxTexCoord texCoord = cxHashGet(this->keys, cxHashStrKey(key));
    if(texCoord == NULL){
        return rv;
    }
    rv.w = texCoord->w;
    rv.h = texCoord->h;
    return rv;
}

void cxTextureBind(cxTexture this)
{
    CX_ASSERT(this != NULL, "bind texture null");
    this->interface->Bind(this);
    if(!this->isSetParam){
        cxOpenGLSetTexParameters(this->texParam);
        this->isSetParam = true;
    }
}

void cxTextureSetParam(cxTexture this,GLuint type,GLuint value)
{
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

