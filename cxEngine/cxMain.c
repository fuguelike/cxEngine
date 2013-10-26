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
#include <core/cxActionXML.h>
#include <views/cxSprite.h>
#include <core/cxViewXML.h>
#include <views/cxButton.h>
#include <json/jansson.h>
#include <views/cxAtlas.h>
#include <views/cxParticle.h>
#include <views/cxTable.h>
#include <core/cxIconv.h>
#include <core/cxFreeType.h>
#include <views/cxLabel.h>
#include <core/cxDB.h>

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

void cxEngineInit(cxEngine engine)
{
    engine->isShowBorder = false;
    cxEngineRegisteEvent("viewLoad", viewLoad);
    cxEngineRegisteEvent("view2Load", view2Load);
    cxEngineRegisteEvent("actionsLoad", actionsLoad);
}

void cxEngineMain(cxEngine engine)
{
    cxBool ret = cxDBEnvOpen();
    CX_ASSERT(ret, "cxDBEnv open error");
    cxEngineDataSet("items.xml");
    
    cxViewXML v = cxViewXMLCreate("main.xml");
    cxWindowPushView(v, NULL);
}

void cxEngineFree(cxEngine engine)
{
    
}








