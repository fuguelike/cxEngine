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
#include <core/cxPlayer.h>
#include <socket/cxHttp.h>
#include <streams/cxMp3Stream.h>
#include <views/cxScroll.h>

void cxEngineInit(cxEngine engine)
{
    //desgin size
    engine->dessize = cxSize2fv(640, 960);
    //prepare load
    cxPlayerOpen(0, 0);
    //open db
    cxDBEnvOpen();
//    engine->isShowBorder = false;
}

void cxEngineMain(cxEngine engine)
{
    cxScroll p = CX_CREATE(cxScroll);
    cxViewSetSize(p, cxSize2fv(500, 700));
    cxViewSetAnchor(p, cxVec2fv(0, 0));
    cxViewSetScale(p, cxVec2fv(0.5f, 0.5f));
    
    cxView c = CX_CREATE(cxView);
    cxViewSetSize(c, cxSize2fv(400, 1000));
    cxViewAppend(p, c);
    
    cxWindowPushView(p, NULL);
    
    cxEngineDataSet("items.xml");
//    cxWindowPushXML("main.xml");
}

void cxEngineFree(cxEngine engine)
{
    
}








