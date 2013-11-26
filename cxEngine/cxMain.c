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
#include <jansson.h>
#include <views/cxAtlas.h>
#include <views/cxParticle.h>
#include <views/cxTable.h>
#include <core/cxIconv.h>
#include <core/cxFreeType.h>
#include <views/cxLabel.h>
#include <core/cxDB.h>
#include <core/cxPlayer.h>
#include <socket/cxHttp.h>
#include <streams/cxMp3Stream.h>
#include <views/cxScroll.h>
#include <actions/cxRunner.h>
#include <views/cxLoading.h>

void cxEngineInit(cxEngine engine)
{
    //desgin size
    engine->dessize = cxSize2fv(640, 960);
    //prepare load
    cxPlayerOpen(0, 0);
//    engine->isShowBorder = false;
}

static void loading(cxAny pview)
{
    cxEngineDataSet("items.xml");
    
    cxAny obj = cxViewXMLCreate("main.xml");
    cxLoadingSetObject(pview, obj);
}

static void finished(cxAny pview)
{
    cxWindowPushView(pview, NULL);
}

void cxEngineMain(cxEngine engine)
{
    cxLoadingStart(loading, finished);
}

void cxEngineFree(cxEngine engine)
{
    
}
















