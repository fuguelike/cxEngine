//
//  cxWindow.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxViewRoot.h>
#include "cxWindow.h"

static cxInt cxWindowLuaPopView(lua_State *L)
{
    cxInt top = lua_gettop(L);
    cxEventArg args = NULL;
    if(top >=1 && lua_isstring(L, 1)){
        args = cxEventArgCreate(lua_tostring(L, 1));
    }
    cxWindowPopView(args);
    return 0;
}

static cxInt cxWindowLuaPushView(lua_State *L)
{
    cxInt top = lua_gettop(L);
    cxEventArg args = NULL;
    if(top >=2 && lua_isstring(L, 2)){
        args = cxEventArgCreate(lua_tostring(L, 2));
    }
    cxAny ret = NULL;
    if(lua_isstring(L, 1)){
        ret = cxWindowPushXML(lua_tostring(L, 1));
    }else{
        cxView view = CX_LUA_GET_PTR(1);
        cxWindowPushView(view, args);
        ret = view;
    }
    CX_LUA_PUSH_OBJECT(ret);
    return 1;
}

static cxInt cxWindowLuaReplaceView(lua_State *L)
{
    cxInt top = lua_gettop(L);
    cxEventArg args = NULL;
    if(top >=2 && lua_isstring(L, 2)){
        args = cxEventArgCreate(lua_tostring(L, 2));
    }
    if(lua_isstring(L, 1)){
        cxWindowReplaceXML(lua_tostring(L, 1));
    }else{
        cxView view = CX_LUA_GET_PTR(1);
        cxWindowReplaceView(view, args);
    }
    return 0;
}

CX_LUA_METHOD_BEGIN(cxWindow)
    {"pushView",cxWindowLuaPushView},
    {"popView",cxWindowLuaPopView},
    {"replaceView",cxWindowLuaReplaceView},
CX_LUA_METHOD_END(cxWindow)

void cxWindowTypeInit()
{
    CX_LUA_LOAD_TYPE(cxWindow);
}

static cxBool cxWindowOnKey(cxAny pview,cxKey *key)
{
    if(key->code == CX_KEYCODE_BACK && key->type == cxKeyTypeUp){
        cxEngineExit();
    }
    return true;
}

CX_OBJECT_INIT(cxWindow, cxView)
{
    CX_METHOD_OVERRIDE(this->super.OnKey, cxWindowOnKey);
    this->views = CX_ALLOC(cxStack);
}
CX_OBJECT_FREE(cxWindow, cxView)
{
    CX_RELEASE(this->views);
}
CX_OBJECT_TERM(cxWindow, cxView)

cxAny cxWindowPushXML(cxConstChars xml)
{
    cxViewRoot rootView = cxViewRootCreate(xml);
    CX_ASSERT(rootView != NULL, "create xml view error : %s",xml);
    cxWindowPushView(rootView, NULL);
    return rootView;
}

cxAny cxWindowReplaceXML(cxConstChars xml)
{
    cxViewRoot rootView = cxViewRootCreate(xml);
    CX_ASSERT(rootView != NULL, "create xml view error : %s",xml);
    cxWindowReplaceView(rootView, NULL);
    return rootView;
}

void cxWindowPopXML()
{
    cxWindowPopView(NULL);
}

cxAny cxWindowTopView()
{
    cxEngine engine = cxEngineInstance();
    return cxStackTop(engine->window->views);
}

void cxWindowPushView(cxAny pview,cxAny args)
{
    CX_ASSERT(pview != NULL, "new view null");
    cxEngine engine = cxEngineInstance();
    cxView new = pview;
    cxView top = cxStackTop(engine->window->views);
    if(top != NULL && new->hideTop){
        cxViewSetVisible(top, false);
        cxViewExit(top);
    }
    cxStackPush(engine->window->views, new);
    cxViewSetArgs(new, args);
    cxViewAppend(engine->window, new);
    cxEngineTimeReset();
}

void cxWindowPopView(cxAny args)
{
    cxEngine engine = cxEngineInstance();
    cxBool hideTop = false;
    cxView prev = cxStackTop(engine->window->views);
    cxStackPop(engine->window->views);
    cxView top = cxStackTop(engine->window->views);
    if(prev != NULL){
        hideTop = prev->hideTop;
        cxViewRemoved(prev);
    }
    if(top != NULL && hideTop){
        cxViewSetArgs(top,args);
        cxViewEnter(top);
        cxViewSetVisible(top, true);
    }
    cxEngineTimeReset();
}

void cxWindowReplaceView(cxAny pview,cxAny args)
{
    CX_ASSERT(pview != NULL, "new view null");
    cxEngine engine = cxEngineInstance();
    cxView top = cxStackTop(engine->window->views);
    if(top != NULL){
        cxViewRemoved(top);
        cxStackPop(engine->window->views);
    }
    cxStackPush(engine->window->views, pview);
    cxViewSetArgs(pview, args);
    cxViewAppend(engine->window, pview);
    cxEngineTimeReset();
}












