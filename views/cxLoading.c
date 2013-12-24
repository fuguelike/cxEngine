//
//  cxLoading.c
//  cxEngineIOS
//
//  Created by xuhua on 11/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <actions/cxTimer.h>
#include "cxLoading.h"

static cxInt cxLoadingLuaSetObject(lua_State *L)
{
    CX_LUA_DEF_THIS(cxLoading);
    CX_LUA_GET_ANY(cxObject, any, 2);
    cxLoadingSetObject(this, any);
    return 0;
}

static cxInt cxLoadingLuaGetObject(lua_State *L)
{
    CX_LUA_DEF_THIS(cxLoading);
    CX_LUA_PUSH_OBJECT(this->object);
    return 1;
}

static cxInt cxLoadingLuaStart(lua_State *L)
{
    CX_LUA_DEF_THIS(cxLoading);
    cxLoadingStart(this);
    return 0;
}

cxInt cxLoadingLuaAppendEvent(lua_State *L)
{
    cxViewLuaAppendEvent(L);
    CX_LUA_DEF_THIS(cxLoading);
    
    CX_LUA_EVENT_BEGIN();
    
    CX_LUA_EVENT_APPEND(onFinished);
    CX_LUA_EVENT_APPEND(onLoading);
    CX_LUA_EVENT_APPEND(onStart);
    
    CX_LUA_EVENT_END();
}

const luaL_Reg cxLoadingInstanceMethods[] = {
    {"start",cxLoadingLuaStart},
    CX_LUA_PROPERTY(cxLoading, Object)
    CX_LUA_ON_EVENT(cxLoading)
    CX_LUA_SUPER(cxView)
};

const luaL_Reg cxLoadingTypeMethods[] = {
    CX_LUA_TYPE(cxLoading)
};

void cxLoadingTypeInit()
{
    CX_LUA_LOAD_TYPE(cxLoading);
}

void cxLoadingOnUpdate(cxEvent *event)
{
    cxLoading this = event->sender;
    if(this->isLoading){
        CX_EVENT_FIRE(this, onFinished);
        cxViewRemoved(this);
    }
}

cxBool cxLoadingTouch(cxAny pview,cxTouch *touch)
{
    return true;
}

void cxLoadingSetObject(cxAny pview,cxAny object)
{
    cxLoading this = pview;
    CX_RETAIN_SWAP(this->object, object);
}

cxAny cxLoadingObject(cxAny pview)
{
    cxLoading this = pview;
    return this->object;
}

static void cxFinishedArrive(cxEvent *event)
{
    cxLoading this = cxActionView(event->sender);
    cxLoaingFinished(this);
}

static void cxLoadingArrive(cxEvent *event)
{
    cxLoading this = cxActionView(event->sender);
    CX_EVENT_FIRE(this, onLoading);
    cxTimer timer = cxViewAppendTimer(this, 1.0f, 1);
    CX_EVENT_QUICK(timer->onArrive, cxFinishedArrive);
}

void cxLoaingFinished(cxAny pview)
{
    cxLoading this = pview;
    this->isLoading = true;
}

CX_OBJECT_INIT(cxLoading, cxView)
{
    this->isLoading = false;
    CX_EVENT_QUICK(this->super.onUpdate, cxLoadingOnUpdate);
    CX_METHOD_SET(this->super.Touch, cxLoadingTouch);
}
CX_OBJECT_FREE(cxLoading, cxView)
{
    CX_EVENT_RELEASE(this->onStart);
    CX_EVENT_RELEASE(this->onFinished);
    CX_EVENT_RELEASE(this->onLoading);
}
CX_OBJECT_TERM(cxLoading, cxView)

void cxLoadingStart(cxLoading this)
{
    cxEngine engine = cxEngineInstance();
    CX_EVENT_FIRE(this, onStart);
    cxTimer timer = cxViewAppendTimer(this, 1.0f, 1);
    CX_EVENT_QUICK(timer->onArrive, cxLoadingArrive);
    cxViewAppend(engine->window, this);
}


