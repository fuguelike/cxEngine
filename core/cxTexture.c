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

CX_OBJECT_INIT(cxTexCoord, cxObject)
{
    
}
CX_OBJECT_FREE(cxTexCoord, cxObject)
{
    
}
CX_OBJECT_TERM(cxTexCoord, cxObject)

static cxInt cxTextureLuaCreateTexture(lua_State *L)
{
    cxConstChars file = luaL_checkstring(L, 1);
    //if need save to cache
    cxBool cache = cxLuaBoolValue(L, 2, true);
    cxTexture texture = cache ? cxTextureFactoryLoadFile(file) : cxTextureCreate(file);
    CX_LUA_PUSH_OBJECT(texture);
    return 1;
}

CX_LUA_METHOD_BEGIN(cxTexture)
    {"create",cxTextureLuaCreateTexture},
CX_LUA_METHOD_END(cxTexture)

void cxTextureTypeInit()
{
    CX_LUA_LOAD_TYPE(cxTexture);
}

CX_OBJECT_INIT(cxTexture, cxObject)
{
    this->texParam = cxtpv(GL_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
//    this->texParam = cxtpv(GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT);
    this->keys = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxTexture, cxObject)
{
    cxOpenGLDeleteTexture(0, this->textureId);
    CX_RELEASE(this->keys);
    CX_METHOD_RELEASE(this->Load);
    CX_METHOD_RELEASE(this->Bind);
}
CX_OBJECT_TERM(cxTexture, cxObject)

void cxTextureSetAtlas(cxTexture this,cxBool isAtlas)
{
    this->isAtlas = isAtlas;
}

void cxTextureLoad(cxTexture this,cxStream stream)
{
    CX_RETURN(this->isLoad);
    this->isLoad = CX_METHOD_FIRE(false, this->Load,this,stream);
    if(!this->isLoad){
        CX_ERROR("texture  can not load");
    }
}

cxBoxTex2f cxTextureBox(cxTexture this,cxConstChars key)
{
    CX_ASSERT(this != NULL, "texture null");
    cxBoxTex2f rv = cxBoxTex2fDefault();
    CX_RETURN(key == NULL, rv);
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
    cxRect4f rv = cxRect4fv(0.0f, 0.0f, 1.0f, 1.0f);
    CX_RETURN(key == NULL, rv);
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
    cxSize2f rv = cxSize2fv(0, 0);
    CX_RETURN(key == NULL, rv);
    rv = this->size;
    cxTexCoord texCoord = cxHashGet(this->keys, cxHashStrKey(key));
    if(texCoord == NULL){
        return rv;
    }
    rv.w = texCoord->w;
    rv.h = texCoord->h;
    return rv;
}

void cxDrawClippingTexture(cxTexture this,const cxVec2f pos,const cxSize2f size,cxConstChars tkey)
{
    cxTextureDraw(this, pos, size, tkey, cxShaderClippingKey);
}

void cxTextureDraw(cxTexture this,const cxVec2f pos,const cxSize2f size,cxConstChars tkey,cxConstChars skey)
{
    cxBoxVec3f vbox;
    cxFloat wh = size.w / 2.0f;
    cxFloat hh = size.h / 2.0f;
    vbox.lb = cxVec3fv(pos.x - wh, pos.y - hh, 0);
    vbox.lt = cxVec3fv(pos.x - wh, pos.y + hh, 0);
    vbox.rb = cxVec3fv(pos.x + wh, pos.y - hh, 0);
    vbox.rt = cxVec3fv(pos.x + wh, pos.y + hh, 0);
    cxBoxColor4f cbox = {
        cxColor4fv(1, 1, 1, 1),
        cxColor4fv(1, 1, 1, 1),
        cxColor4fv(1, 1, 1, 1),
        cxColor4fv(1, 1, 1, 1)
    };
    cxBoxTex2f tbox = cxBoxTex2fDefault();
    if(tkey != NULL){
        tbox = cxTextureBox(this, tkey);
    }
    cxTextureBind(this);
    cxOpenGLSetBlendFactor(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxOpenGLUsingShader(skey,this->isAtlas);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxVec3f), &vbox);
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxTexCoord2f), &tbox);
    glVertexAttribPointer(cxVertexAttribColor,    4, GL_FLOAT, GL_FALSE, sizeof(cxColor4f), &cbox);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void cxTextureBind(cxTexture this)
{
    CX_ASSERT(this != NULL, "bind texture null");
    CX_METHOD_FIRE(NULL, this->Bind,this);
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




