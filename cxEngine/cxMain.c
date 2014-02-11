//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxDB.h>

#include <core/cxViewRoot.h>
#include <views/cxChipmunk.h>

static cxBool chipmunkTouch(cxAny pview,cxTouch *touch)
{
    if(touch->type == cxTouchTypeUp){
        cxChipmunk chipmunk = pview;
        cxChipmunkAttr *attr = cxChipmunkAttrCreate(false);
        attr->e = 0.9f;
        attr->shape = cxChipmunkShapeCircle;
        cxSprite sprite = cxSpriteCreateWithURL("ball.png");
        cxChipmunkAppend(chipmunk, sprite, attr);
        cxChipmunkSetPos(sprite, touch->current);
    }
    return false;
}

static cxInt chipmunkOnEnd(lua_State *L)
{
    cxEngine engine = cxEngineInstance();
    
    cxViewRoot root = CX_LUA_GET_PTR(1);
    cxChipmunk chipmunk = cxViewRootGet(root, "chipmunk");
    CX_ASSERT(chipmunk != NULL, "get chipmunk error,xml not define chipmunk");
    
    CX_METHOD_OVERRIDE(chipmunk->super.Touch, chipmunkTouch);
    
    cxChipmunkAttr *attr = cxChipmunkAttrCreate(true);
    attr->e = 1.0f;
    //up
    {
        cxView view = CX_CREATE(cxView);
        cxViewSetSize(view, cxSize2fv(engine->winsize.w, 5));
        cxViewSetPos(view, cxVec2fv(0, engine->winsize.h/2));
        cxChipmunkAppend(chipmunk, view, attr);
    }
    //bottom
    {
        cxView view = CX_CREATE(cxView);
        cxViewSetSize(view, cxSize2fv(engine->winsize.w, 5));
        cxViewSetPos(view, cxVec2fv(0, -engine->winsize.h/2));
        cxChipmunkAppend(chipmunk, view, attr);
    }
    //left
    {
        cxView view = CX_CREATE(cxView);
        cxViewSetSize(view, cxSize2fv(5, engine->winsize.h));
        cxViewSetPos(view, cxVec2fv(-engine->winsize.w/2, 0));
        cxChipmunkAppend(chipmunk, view, attr);
    }
    //right
    {
        cxView view = CX_CREATE(cxView);
        cxViewSetSize(view, cxSize2fv(5, engine->winsize.h));
        cxViewSetPos(view, cxVec2fv(engine->winsize.w/2, 0));
        cxChipmunkAppend(chipmunk, view, attr);
    }
    return 0;
}


void cxEngineInit(cxEngine engine)
{
    cxEngineRegisteFunc(chipmunkOnEnd);
    
    cxEngineTypes("items.xml");
    
    cxEngineLuaRunChars("require('main')");
    
    cxEngineLuaRunChars("init()");
}

void cxEngineMain(cxEngine engine)
{
    cxEngineLuaRunChars("main()");
}

void cxEngineFree(cxEngine engine)
{
    cxEngineLuaRunChars("free()");
}
















