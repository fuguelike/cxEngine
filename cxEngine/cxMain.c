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


#include <actions/cxMove.h>
#include <actions/cxCurve.h>
#include <core/cxActionXML.h>
#include <views/cxSprite.h>
#include <core/cxViewXML.h>
#include <views/cxButton.h>
#include <json/jansson.h>
#include <views/cxAtlas.h>
#include <views/cxParticle.h>

static void cxEventTest(cxAny view,cxAny arg)
{
    cxActionXMLAttachView(view, "actions.xml", "move");
}

static void viewLoad(cxAny pview,cxAny arg)
{
    //pview == cxViewXML
    //add view custom event
    cxViewXMLRegisteEvent(pview, "cxEventTest",cxEventTest);
}

static void view2Load(cxAny pview,cxAny arg)
{
    //pview == cxViewXML
    //add view custom event
//    cxViewXMLRegisteEvent(pview, "cxEventTest",cxEventTest);
}

static void actionsLoad(cxAny pav,cxAny arg)
{
    //pav == cxActionXML
    //add action custom event
}

cxBool cxEngineInit(cxEngine engine)
{
//    engine->isShowBorder = false;
    cxEngineRegisteEvent("viewLoad", viewLoad);
    cxEngineRegisteEvent("view2Load", view2Load);
    cxEngineRegisteEvent("actionsLoad", actionsLoad);
    return true;
}

void cxEngineMain(cxEngine engine)
{
//    cxViewXML v = cxViewXMLCreate("view.xml");
//    cxViewAppend(engine->window, v);
    
    cxParticle p = CX_ALLOC(cxParticle);
    cxTexture texture = cxTextureLoadFile("candy.xml");
    cxSpriteSetTexture(p, texture);
    cxSpriteSetTextureKey(p, "green.png", true);
    cxParticleInit(p, 500);
    
    p->duration = -1;
    p->life = cxFloatRangeValue(5,0);
    p->speed = cxFloatRangeValue(180,100);
    p->angle = cxFloatRangeValue(90,45);
    p->gravity = cxVec2fv(0, -100);
    p->todir = true;
    p->startspin = cxFloatRangeValue(2000,0);
    p->startsize = cxFloatRangeValue(45,0);
    p->endsize = cxFloatRangeValue(30,0);
    p->startcolor = cxColorRangeValue(1,1,1,1,0,0,0,0);
    p->endcolor = cxColorRangeValue(1,1,1,1,0,0,0,0);
    p->rate = 50;
    
    cxViewAppend(engine->window, p);
}

void cxEngineFree(cxEngine engine)
{
    
}








