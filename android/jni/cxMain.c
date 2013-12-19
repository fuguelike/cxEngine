//
//  cxMain.c
//  android
//
//  Created by xuhua on 12/14/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxViewRoot.h>
#include <core/cxEngine.h>
#include <views/cxChipmunk.h>
#include <views/cxLoading.h>
#include <views/cxLabelTTF.h>
#include <luajit/cxLuaLoader.h>
#include <views/cxParticle.h>

static cxBool cxMainViewOnKey(cxAny pview,cxKey *key)
{
    CX_LOGGER("main on key %d",key->type);
    return false;
}

static cxAny loading(cxAny loading)
{
    cxEngineDataSet("items.xml");
    cxViewRoot root = cxViewRootCreate("main.xml");
    CX_METHOD_SET(root->super.OnKey, cxMainViewOnKey);
    return root;
}

static void appendParticle(cxEvent *event)
{
    cxAny view = cxObjectRoot(event->sender);
    cxParticle new = cxParticleCreateFromPEX("particle.pex");
    new->autoRemove = true;
    new->duration = 2;
    cxViewAppend(view, new);
}

static void finished(cxAny pview)
{
    cxWindowPushView(pview, NULL);
}

static void deleteSprite(cxEvent *event)
{
    cxAny pview = cxViewRootGet(event->sender, "purple");
    cxChipmunkApplyImpulse(pview, cxVec2fv(1000, 1000), cxVec2fv(0, 1));
}

static void cxChipmunkBegin(cxEvent *event)
{
    cxViewRootRegisteEvent(event->sender, "deleteSprite", deleteSprite);
}

void cxEngineInit(cxEngine engine)
{
    cxLuaLoader lua = CX_ALLOC(cxLuaLoader);
    cxLuaLoaderRun(lua, UTF8("require('aaa.lua')"));
    cxLuaLoaderRun(lua, UTF8("test()"));
    
    cxEngineRegisteEvent("appendParticle", appendParticle);
    cxEngineRegisteEvent("cxChipmunkBegin", cxChipmunkBegin);
    
    engine->dessize = cxSize2fv(640, 960);
    cxPlayerOpen(0, 0);
    
    //    engine->isShowBorder = false;
}

void cxEngineMain(cxEngine engine)
{
    cxAny lv = cxLoadingStart(loading, finished);
    cxLabelTTF txt = cxLabelTTFCreate(UTF8("Loading..."), UTF8("banana.ttf"), 60);
    cxViewSetColor(txt, cxRED);
    cxViewAppend(lv, txt);
}

void cxEngineFree(cxEngine engine)
{
    
}
