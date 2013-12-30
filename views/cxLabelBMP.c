//
//  cxLabelBMP.c
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxIconv.h>
#include <core/cxViewRoot.h>
#include "cxLabelBMP.h"

static cxInt cxLabelBMPLuaSetFont(lua_State *L)
{
    CX_LUA_DEF_THIS(cxLabelBMP);
    if(!lua_istable(L, 2)){
        luaL_error(L, "args error: {font='',size=''}");
        return 0;
    }
    lua_getfield(L, 2, "font");
    if(lua_isstring(L, -1)){
        cxLabelBMPSetFont(this, UTF8(luaL_checkstring(L, -1)));
    }
    lua_pop(L, 1);
    
    lua_getfield(L, 2, "size");
    if(lua_isnumber(L, -1)){
        cxLabelBMPSetSize(this, luaL_checknumber(L, -1));
    }
    lua_pop(L, 1);
    return 0;
}

static cxInt cxLabelBMPLuaSetText(lua_State *L)
{
    CX_LUA_DEF_THIS(cxLabelBMP);
    cxConstChars text = luaL_checkstring(L, 2);
    cxLabelBMPSetText(this, UTF8(text));
    return 0;
}

static cxInt cxLabelBMPLuaMake(lua_State *L)
{
    CX_LUA_CREATE_THIS(cxLabelBMP);
    if(lua_istable(L, 1)){
        lua_getfield(L, 1, "font");
        if(lua_isstring(L, -1)){
            cxLabelBMPSetFont(this, UTF8(lua_tostring(L, -1)));
        }
        lua_pop(L, 1);
        lua_getfield(L, 1, "size");
        if(lua_isnumber(L, -1)){
            cxLabelBMPSetSize(this, lua_tonumber(L, -1));
        }
        lua_pop(L, 1);
        lua_getfield(L, 1, "text");
        if(lua_isstring(L, -1)){
            cxLabelBMPSetText(this, UTF8(lua_tostring(L, -1)));
        }
        lua_pop(L, 1);
    }
    CX_LUA_RET_THIS(cxLabelBMP);
}

CX_LUA_METHOD_BEG(cxLabelBMP)
    {"SetFont",cxLabelBMPLuaSetFont},
    {"SetText",cxLabelBMPLuaSetText},
    {"Make",cxLabelBMPLuaMake},
CX_LUA_METHOD_END(cxLabelBMP)

void cxLabelBMPTypeInit()
{
    CX_LUA_LOAD_TYPE(cxLabelBMP);
}

static void cxLabelBMPUpdateText(cxLabelBMP this)
{
    CX_RETURN(!cxStringLength(this->txt));
    cxTexture texture = cxBMPFontTexture(this->font, 0);
    cxSpriteSetTexture(this, texture);
    cxString unicode = cxIconvConvertUTF8ToUTF16(this->txt);
    cxInt number = cxStringLength(unicode) / 2;
    cxAtlasSetNumber(this, number);
    cxAtlasClean(this);
    //get size
    cxSize2f size = cxViewSize(this);
    cxInt width = 0;
    cxInt w = 0;
    cxBMPElement pchar = NULL;
    for(cxInt i=0; i < cxStringLength(unicode);i += 2){
        uint16_t code = *(uint16_t *)&unicode->strptr.d[i];
        pchar = cxBMPFontChar(this->font, code);
        if(pchar == NULL){
            continue;
        }
        w += pchar->xadvance;
        if(width < w){
            width = w;
        }
    }
    size.h = this->font->lineHeight;
    if(pchar->xadvance < pchar->box.w){
        size.w = width + pchar->box.w - pchar->xadvance;
    }else{
        size.w = width;
    }
    cxViewSetSize(this, size);
    //
    cxInt x = -size.w/2.0f;
    cxInt y = 0;
    cxVec2f fontpos = cxVec2fv(0, 0);
    for(cxInt i=0; i < cxStringLength(unicode);i += 2){
        uint16_t code = *(uint16_t *)&unicode->strptr.d[i];
        cxBMPElement pchar = cxBMPFontChar(this->font, code);
        if(pchar == NULL){
            CX_WARN("bmp font char %d not exists",code);
            continue;
        }
        cxInt yoff = this->font->lineHeight / 2.0f - pchar->yoffset;
        fontpos.x = x + pchar->xoffset + pchar->box.w / 2.0f;
        fontpos.y = y + yoff - pchar->box.h / 2.0f;
        cxSize2f size = cxSize2fv(pchar->box.w, pchar->box.h);
        cxBoxTex2f tex = cxRect4fToBoxTex2f(pchar->box, this->font->scale);
        cxColor4f color = cxViewColor(this);
        cxBoxPoint bp = cxAtlasCreateBoxPoint(fontpos, size, tex, color);
        cxAtlasAppend(this, bp);
        x += pchar->xadvance;
    }
}

static void cxLabelBMPUpdate(cxEvent *event)
{
    cxLabelBMP this = event->sender;
    CX_RETURN(!this->isDirty);
    cxLabelBMPUpdateText(this);
    this->isDirty = false;
}

void cxLabelBMPReadAttr(cxReaderAttrInfo *info)
{
    cxSpriteReadAttr(info);
    cxLabelBMP this = info->object;
    //set font
    cxString font = cxXMLReadStringAttr(info, "cxLabelBMP.font");
    cxLabelBMPSetFont(this, font);
    //set text
    cxString text = cxXMLReadStringAttr(info, "cxLabelBMP.text");
    cxLabelBMPSetText(this, text);
    //set fontsize
    cxLabelBMPSetSize(this, cxXMLReadFloatAttr(info, "cxLabelBMP.size", this->size));
}

CX_OBJECT_INIT(cxLabelBMP, cxAtlas)
{
    cxObjectSetReadAttrFunc(this, cxLabelBMPReadAttr);
    CX_EVENT_QUICK(this->super.super.super.onUpdate, cxLabelBMPUpdate);
    this->isDirty = true;
}
CX_OBJECT_FREE(cxLabelBMP, cxAtlas)
{
    CX_RELEASE(this->font);
    CX_RELEASE(this->txt);
}
CX_OBJECT_TERM(cxLabelBMP, cxAtlas)

void cxLabelBMPSetSize(cxAny pview,cxFloat size)
{
    cxLabelBMP this = pview;
    CX_RETURN(cxFloatEqu(this->size,size));
    this->size = size;
    this->isDirty = true;
}

void cxLabelBMPSetFont(cxAny pview,cxString font)
{
    cxLabelBMP this = pview;
    cxBMPFont bmpfont = cxEngineLoadBMPFont(cxStringBody(font));
    CX_ASSERT(bmpfont != NULL, "%s font not load",font);
    CX_RETURN(bmpfont == this->font);
    CX_RETAIN_SWAP(this->font, bmpfont);
    this->isDirty = true;
}

void cxLabelBMPSetText(cxAny pview,cxString txt)
{
    cxLabelBMP this = pview;
    CX_RETURN(cxStringEqu(this->txt, txt));
    CX_RETAIN_SWAP(this->txt, txt);
    this->isDirty = true;
}








