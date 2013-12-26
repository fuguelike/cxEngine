//
//  cxButton.c
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxViewRoot.h>
#include <core/cxXMLScript.h>
#include "cxButton.h"

static cxInt cxButtonLuaAppendEvent(lua_State *L)
{
    CX_LUA_DEF_THIS(cxButton);
    
    CX_LUA_EVENT_BEGIN();
    
    CX_LUA_EVENT_APPEND(onEnter);
    CX_LUA_EVENT_APPEND(onLeave);
    CX_LUA_EVENT_APPEND(onPress);
    CX_LUA_EVENT_APPEND(onRelease);
    
    CX_LUA_EVENT_END();
}

CX_LUA_METHOD_BEGIN(cxButton)
    {"event",cxButtonLuaAppendEvent},
CX_LUA_METHOD_END(cxButton)

void cxButtonTypeInit()
{
    CX_LUA_LOAD_TYPE(cxButton);
}

cxBool cxButtonTouch(cxAny pview,cxTouch *touch)
{
    cxButton this = pview;
    if(!this->isEnable){
        return false;
    }
    cxBool hit = cxViewHitTest(pview, touch->current, NULL);
    if(!hit && this->isDown){
        CX_EVENT_FIRE(this, onLeave);
        this->isDown = false;
    }
    if(!hit){
        return false;
    }
    if(touch->type == cxTouchTypeDown){
        this->isDown = true;
        CX_EVENT_FIRE(this, onEnter);
        CX_EVENT_FIRE(this, onPress);
        return true;
    }
    if(!this->isDown){
        return false;
    }
    if(touch->type == cxTouchTypeMove && cxVec2fMagnitude(touch->movement) > this->movement){
        CX_EVENT_FIRE(this, onLeave);
        this->isDown = false;
        return false;
    }
    if(touch->type == cxTouchTypeUp){
        this->isDown = false;
        CX_EVENT_FIRE(this, onRelease);
        CX_EVENT_FIRE(this, onLeave);
        return true;
    }
    return false;
}

void cxButtonReadAttr(cxReaderAttrInfo *info)
{
    cxButton this = info->object;
    cxSpriteReadAttr(info);
    cxXMLAppendEvent(info, this, cxButton, onPress);
    cxXMLAppendEvent(info, this, cxButton, onRelease);
    cxXMLAppendEvent(info, this, cxButton, onLeave);
    cxXMLAppendEvent(info, this, cxButton, onEnter);
    cxButtonEnable(this, cxXMLReadBoolAttr(info, "cxButton.enable", this->isEnable));
    this->movement = cxXMLReadFloatAttr(info,  "cxButton.movement", this->movement);
}

CX_OBJECT_INIT(cxButton, cxSprite)
{
    cxObjectSetReadAttrFunc(this, cxButtonReadAttr);
    this->movement = 25;
    this->isEnable = true;
    CX_METHOD_OVERRIDE(this->super.super.Touch, cxButtonTouch);
}
CX_OBJECT_FREE(cxButton, cxSprite)
{
    CX_EVENT_RELEASE(this->onEnter);
    CX_EVENT_RELEASE(this->onLeave);
    CX_EVENT_RELEASE(this->onPress);
    CX_EVENT_RELEASE(this->onRelease);
}
CX_OBJECT_TERM(cxButton, cxSprite)

void cxButtonEnable(cxAny pview,cxBool enable)
{
    cxButton this = pview;
    this->isEnable = enable;
}


