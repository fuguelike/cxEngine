//
//  cxLabelTTF.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include <core/cxViewRoot.h>
#include <textures/cxTextureFactory.h>
#include "cxLabelTTF.h"

static cxInt cxLabelTTFLuaSetFont(lua_State *L)
{
    CX_LUA_DEF_THIS(cxLabelTTF);
    if(!lua_istable(L, 2)){
        luaL_error(L, "args error: {font='',size=''}");
        return 0;
    }
    lua_getfield(L, 2, "font");
    if(lua_isstring(L, -1)){
        cxLabelTTFSetFont(this, UTF8(luaL_checkstring(L, -1)));
    }
    lua_pop(L, 1);
    
    lua_getfield(L, 2, "size");
    if(lua_isnumber(L, -1)){
        cxLabelTTFSetFontSize(this, luaL_checknumber(L, -1));
    }
    lua_pop(L, 1);
    return 0;
}

static cxInt cxLabelTTFLuaSetText(lua_State *L)
{
    CX_LUA_DEF_THIS(cxLabelTTF);
    cxConstChars text = luaL_checkstring(L, 2);
    cxLabelTTFSetText(this, UTF8(text));
    return 0;
}

static cxInt cxLabelTTFLuaMake(lua_State *L)
{
    CX_LUA_CREATE_THIS(cxLabelTTF);
    if(lua_istable(L, 1)){
        lua_getfield(L, 1, "font");
        if(lua_isstring(L, -1)){
            cxLabelTTFSetFont(this, UTF8(lua_tostring(L, -1)));
        }
        lua_pop(L, 1);
        lua_getfield(L, 1, "size");
        if(lua_isnumber(L, -1)){
            cxLabelTTFSetFontSize(this, lua_tonumber(L, -1));
        }
        lua_pop(L, 1);
        lua_getfield(L, 1, "text");
        if(lua_isstring(L, -1)){
            cxLabelTTFSetText(this, UTF8(lua_tostring(L, -1)));
        }
        lua_pop(L, 1);
    }
    CX_LUA_RET_THIS(cxLabelTTF);
}

CX_LUA_METHOD_BEG(cxLabelTTF)
    {"SetFont",cxLabelTTFLuaSetFont},
    {"SetText",cxLabelTTFLuaSetText},
    {"Make",cxLabelTTFLuaMake},
CX_LUA_METHOD_END(cxLabelTTF)

void cxLabelTTFTypeInit()
{
    CX_LUA_LOAD_TYPE(cxLabelTTF);
}

void cxLabelTTFReadAttr(cxReaderAttrInfo *info)
{
    cxSpriteReadAttr(info);
    cxLabelTTF this = info->object;
    //set font
    cxString font = cxXMLReadStringAttr(info, "cxLabelTTF.font");
    cxLabelTTFSetFont(this, font);
    //set text
    cxString text = cxXMLReadStringAttr(info, "cxLabelTTF.text");
    cxLabelTTFSetText(this, text);
    //set fontsize
    cxLabelTTFSetFontSize(this, cxXMLReadFloatAttr(info, "cxLabelTTF.size", this->attr.size));
    //set fontbold
    cxLabelTTFSetFontBold(this, cxXMLReadBoolAttr(info, "cxLabelTTF.bold", this->attr.bold));
}

static void cxLabelTTFUpdate(cxEvent *event)
{
    cxLabelTTF this = event->sender;
    CX_RETURN(!this->isDirty);
    cxLabelTTFUpdateText(this);
    this->isDirty = false;
}

CX_OBJECT_INIT(cxLabelTTF, cxSprite)
{
    cxObjectSetReadAttrFunc(this, cxLabelTTFReadAttr);
    cxViewOnUpdate(this, cxLabelTTFUpdate);
    this->attr.size = 32;
    cxSpriteSetShader(this, cxShaderAlphaKey);
}
CX_OBJECT_FREE(cxLabelTTF, cxSprite)
{
    CX_RELEASE(this->font);
    CX_RELEASE(this->text);
}
CX_OBJECT_TERM(cxLabelTTF, cxSprite)

cxLabelTTF cxLabelTTFCreate(cxString txt,cxString font,cxFloat fontsize)
{
    cxLabelTTF this = CX_CREATE(cxLabelTTF);
    cxLabelTTFSetFont(this, font);
    cxLabelTTFSetText(this, txt);
    cxLabelTTFSetFontSize(this, fontsize);
    return this;
}

void cxLabelTTFUpdateText(cxAny pview)
{
    cxLabelTTF this = pview;
    cxTexture texture = cxTextureFactoryLoadText(this->text, this->font, this->attr);
    CX_RETURN(texture == NULL);
    cxSpriteSetTexture(this, texture);
    cxViewSetSize(this, texture->size);
}

void cxLabelTTFSetFontBold(cxAny pview,cxBool bold)
{
    cxLabelTTF this = pview;
    CX_RETURN(this->attr.bold == bold);
    this->attr.bold = bold;
    this->isDirty = true;
}

void cxLabelTTFSetFontSize(cxAny pview,cxFloat fontsize)
{
    cxLabelTTF this = pview;
    CX_RETURN(kmAlmostEqual(this->attr.size, fontsize));
    this->attr.size = fontsize;
    this->isDirty = true;
}

void cxLabelTTFSetFont(cxAny pview,cxString font)
{
    cxLabelTTF this = pview;
    CX_RETURN(cxStringEqu(this->font, font));
    CX_RETAIN_SWAP(this->font, font);
    this->isDirty = true;
}

void cxLabelTTFSetText(cxAny pview,cxString txt)
{
    cxLabelTTF this = pview;
    CX_RETURN(cxStringEqu(this->text, txt));
    CX_RETAIN_SWAP(this->text, txt);
    this->isDirty = true;
}


