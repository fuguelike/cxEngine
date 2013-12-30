//
//  cxSprite.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxViewRoot.h>
#include <core/cxOpenGL.h>
#include <core/cxXMLScript.h>
#include <textures/cxTextureFactory.h>
#include "cxSprite.h"

static cxInt cxSpriteLuaSetTexture(lua_State *L)
{
    CX_LUA_DEF_THIS(cxSprite);
    cxBool ust = cxLuaBoolValue(L, 3, true);
    cxBool cache = cxLuaBoolValue(L, 4, true);
    cxConstChars url = luaL_checkstring(L, 2);
    cxSpriteSetTextureURL(this, url, ust, cache);
    return 0;
}

static cxInt cxSpriteLuaMake(lua_State *L)
{
    CX_LUA_CREATE_THIS(cxSprite);
    cxBool ust = cxLuaBoolValue(L, 2, true);
    cxBool cache = cxLuaBoolValue(L, 3, true);
    cxConstChars url = luaL_checkstring(L, 1);
    cxSpriteSetTextureURL(this, url, ust, cache);
    CX_LUA_RET_THIS(cxSprite);
}

CX_LUA_METHOD_BEG(cxSprite)
    {"SetTexture",cxSpriteLuaSetTexture},
    {"Make",cxSpriteLuaMake},
CX_LUA_METHOD_END(cxSprite)


void cxSpriteTypeInit()
{
    CX_LUA_LOAD_TYPE(cxSprite);
}

void cxSpriteSetBlendFactor(cxAny pview,GLenum sfactor, GLenum dfactor)
{
    cxSprite this = pview;
    this->sfactor = sfactor;
    this->dfactor = dfactor;
}

void cxSpriteDraw(cxAny pview)
{
    cxSprite this = pview;
    CX_RETURN(this->texture == NULL);
    cxOpenGLSetBlendFactor(this->sfactor, this->dfactor);
    cxShaderUsing(this->shader,this->texture->isAtlas);
    cxTextureBind(this->texture);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxVec3f), &this->vbox);
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxTexCoord2f), &this->tbox);
    glVertexAttribPointer(cxVertexAttribColor,    4, GL_FLOAT, GL_FALSE, sizeof(cxColor4f), &this->cbox);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static void cxSpriteDirtyEvent(cxEvent *event)
{
    cxSprite this = event->sender;
    this->cbox.lb = this->super.color;
    this->cbox.rb = this->super.color;
    this->cbox.lt = this->super.color;
    this->cbox.rt = this->super.color;
    cxBox4f box = cxViewBox(this);
    this->vbox.lb = cxVec3fv(box.l, box.b, 0.0f);
    this->vbox.rb = cxVec3fv(box.r, box.b, 0.0f);
    this->vbox.lt = cxVec3fv(box.l, box.t, 0.0f);
    this->vbox.rt = cxVec3fv(box.r, box.t, 0.0f);
    this->tbox = this->texCoord;
    if(this->isFlipX){
        this->tbox.lb.u = this->texCoord.rb.u;
        this->tbox.rb.u = this->texCoord.lb.u;
        this->tbox.lt.u = this->texCoord.rt.u;
        this->tbox.rt.u = this->texCoord.lt.u;
    }
    if(this->isFlipY){
        this->tbox.lb.v = this->texCoord.lt.v;
        this->tbox.rb.v = this->texCoord.rt.v;
        this->tbox.lt.v = this->texCoord.lb.v;
        this->tbox.rt.v = this->texCoord.rb.v;
    }
}

void cxSpriteSetFlipX(cxAny pview,cxBool flipx)
{
    cxSprite this = pview;
    this->isFlipX = flipx;
    cxViewSetDirty(pview, true);
}

void cxSpriteSetFlipY(cxAny pview,cxBool flipy)
{
    cxSprite this = pview;
    this->isFlipY = flipy;
    cxViewSetDirty(pview, true);
}

void cxSpriteSetTextureURL(cxAny pview,cxConstChars url,cxBool useTexSize,cxBool cached)
{
    CX_RETURN(url == NULL);
    cxSprite this = pview;
    cxUrlPath path = cxUrlPathParse(url);
    CX_RETURN(path == NULL);
    cxTexture texture = cxTextureFactoryLoadFile(path->path);
    if(cached){
        texture = cxTextureFactoryLoadFile(path->path);
    }else{
        texture = cxTextureCreate(path->path);
    }
    CX_ASSERT(texture != NULL, "texture load failed %s",path->path);
    cxSpriteSetTexture(this, texture);
    //use texture size
    cxBool uts = cxViewZeroSize(this) || useTexSize;
    if(path->count > 1){
        cxSpriteSetTextureKey(this, path->key, uts);
    }else if(uts){
        cxViewSetSize(this, texture->size);
    }
}

//texture="res/a.xml?green.png"
void cxSpriteReadAttr(cxReaderAttrInfo *info)
{
    cxSprite this = info->object;
    //set texture
    cxTextureAttr texAttr = cxXMLReadTextureAttr(info, "cxSprite.texture");
    cxSpriteSetTextureAttr(this, texAttr);
    //flipx flipy
    cxSpriteSetFlipX(this, cxXMLReadBoolAttr(info, "cxSprite.flipX", this->isFlipX));
    cxSpriteSetFlipY(this, cxXMLReadBoolAttr(info, "cxSprite.flipY", this->isFlipY));
    //shader
    cxString shader = cxXMLReadStringAttr(info, "cxSprite.shader");
    cxSpriteSetShader(this, cxStringBody(shader));
    //
    cxViewReadAttr(info);
}

CX_OBJECT_INIT(cxSprite, cxView)
{
    this->texCoord = cxBoxTex2fDefault();
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CX_EVENT_QUICK(this->super.onDirty, cxSpriteDirtyEvent);
    CX_METHOD_OVERRIDE(this->super.Draw, cxSpriteDraw);
    cxSpriteSetShader(this, cxShaderDefaultKey);
    cxObjectSetReadAttrFunc(this, cxSpriteReadAttr);
}
CX_OBJECT_FREE(cxSprite, cxView)
{
    CX_RELEASE(this->shader);
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxSprite, cxView)

cxSprite cxSpriteCreateWithFile(cxConstChars file,cxConstChars key)
{
    cxSprite this = CX_CREATE(cxSprite);
    cxTexture texture = cxTextureFactoryLoadFile(file);
    cxSpriteSetTexture(this, texture);
    cxSpriteSetTextureKey(this, key, true);
    return this;
}

void cxSpriteSetTextureAttr(cxAny pview,cxTextureAttr attr)
{
    CX_RETURN(attr == NULL);
    cxSpriteSetTexture(pview, attr->texture);
    cxSpriteSetBoxTex(pview, attr->box);
    //
    if(cxViewZeroSize(pview)){
        cxViewSetSize(pview, attr->size);
    }
}

void cxSpriteSetBoxTex(cxAny pview,cxBoxTex2f box)
{
    cxSprite this = pview;
    this->texCoord = box;
    cxViewSetDirty(this, true);
}

cxBoxTex2f cxSpriteBoxTex(cxAny pview)
{
    cxSprite this = pview;
    return this->texCoord;
}

void cxSpriteSetTextureKey(cxAny pview,cxConstChars key,cxBool equSize)
{
    cxSprite this = pview;
    if(this->texture == NULL){
        cxTexture texture = cxTextureFactoryLoadFile(key);
        cxSpriteSetTexture(pview, texture);
    }else{
        this->texCoord = cxTextureBox(this->texture, key);
    }
    CX_ASSERT(this->texture != NULL, "sprite texture not load");
    if(equSize){
        cxViewSetSize(pview, cxTextureSize(this->texture, key));
    }
    this->super.isDirty = true;
}

void cxSpriteSetShader(cxAny pview,cxConstChars key)
{
    CX_RETURN(key == NULL);
    cxShader shader = cxOpenGLShader(key);
    CX_RETURN(shader == NULL);
    cxSprite this = pview;
    CX_RETAIN_SWAP(this->shader, shader);
}

void cxSpriteSetTexture(cxAny pview,cxTexture texture)
{
    cxSprite this = pview;
    CX_RETAIN_SWAP(this->texture, texture);
}


