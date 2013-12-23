//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxHash.h>
#include <streams/cxAssetsStream.h>
#include <textures/cxTexturePVR.h>
#include <textures/cxTexturePNG.h>
#include <textures/cxTextureFactory.h>
#include "cxMain.h"
#include <core/cxDB.h>
#include <actions/cxMove.h>
#include <actions/cxCurve.h>
#include <core/cxActionRoot.h>
#include <views/cxSprite.h>
#include <core/cxViewRoot.h>
#include <views/cxButton.h>
#include <views/cxAtlas.h>
#include <views/cxParticle.h>
#include <views/cxTable.h>
#include <core/cxIconv.h>
#include <core/cxFreeType.h>
#include <views/cxLabelTTF.h>
#include <core/cxDB.h>
#include <core/cxPlayer.h>
#include <socket/cxHttp.h>
#include <views/cxScroll.h>
#include <actions/cxRunner.h>
#include <views/cxLoading.h>
#include <views/cxChipmunk.h>
#include <views/cxLabelBMP.h>
#include <core/cxBMPFont.h>

//static cxBool cxMainViewOnKey(cxAny pview,cxKey *key)
//{
//    CX_LOGGER("main on key %d",key->type);
//    return true;
//}
//
//static cxAny loading(cxAny loading)
//{
//    cxEngineDataSet("items.xml");
//    cxViewRoot root = cxViewRootCreate("main.xml");
//    CX_METHOD_SET(root->super.OnKey, cxMainViewOnKey);
//    return root;
//}
//
//static void finished(cxAny pview)
//{
//    cxWindowPushView(pview, NULL);
//}
//
//static void appendParticle(cxEvent *event)
//{
//    cxAny view = cxObjectRoot(event->sender);
//    cxParticle new = cxParticleCreateFromPEX("particle.pex");
//    new->autoRemove = true;
//    new->duration = 2;
//    cxViewAppend(view, new);
//}
//
//static void deleteSprite(cxEvent *event)
//{
//    cxAny pview = cxViewRootGet(event->sender, "purple");
//    cxChipmunkApplyImpulse(pview, cxVec2fv(1000, 1000), cxVec2fv(0, 1));
//}
//
//static void cxChipmunkBegin(cxEvent *event)
//{
//    cxViewRootRegisteEvent(event->sender, "deleteSprite", deleteSprite);
//}

void cxEngineInit(cxEngine engine)
{
    
    cxEngineLuaRunChars("require('main.lua')");
    cxEngineLuaRunChars("init()");
    
//    cxEngineRegisteEvent("appendParticle", appendParticle);
//    cxEngineRegisteEvent("cxChipmunkBegin", cxChipmunkBegin);
//    
//    engine->dessize = cxSize2fv(640, 960);
    
//    engine->isShowBorder = false;
}

void cxEngineMain(cxEngine engine)
{
//    cxAny lv = cxLoadingStart(loading, finished);
//    cxLabelTTF txt = cxLabelTTFCreate(UTF8("Loading..."), UTF8("banana.ttf"), 60);
//    cxViewSetColor(txt, cxRED);
//    cxViewAppend(lv, txt);
    
    cxEngineLuaRunChars("main()");
}

void cxEngineFree(cxEngine engine)
{
    cxEngineLuaRunChars("free()");
}
















