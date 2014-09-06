//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>

void cxEngineType(cxEngine engine)
{
    
}

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(640, 960));
    engine->isShowBorder = false;
}

#include <actions/cxAnimate.h>
#include <actions/cxScale.h>
#include <actions/cxMove.h>
#include <actions/cxRotate.h>
//#include <views/cxAtlas.h>
#include <views/cxLayer.h>
//cxAtlas x;
//
//static cxBool touch(cxAny pview,cxTouchItems *points)
//{
//    cxTouchItem item = points->items[0];
//    if(item->type== cxTouchTypeDown){
//        cxAtlasRemoveAt(x, 0);
//    }
//    return false;
//}

void cxEngineMain(cxEngine engine)
{
//    CX_METHOD_SET(engine->window->cxView.Touch, touch);
//    cxLoader loader = cxLoaderCreate("cxAtlas.json");
//    cxWindowPushView(loader->object);
//    x = cxLoaderGet(loader, "v0");
//    CX_ASSERT( x != NULL, "get x error");
    
    cxLayer l = CX_CREATE(cxLayer);
    cxSpriteSetTextureURL(l, "texture.json", false);
    
    cxSprite s1 = cxSpriteCreateWithURL("texture.json?cholesterol.png");
    cxMove m = cxMoveCreate(3.0f, cxVec2fv(500, 500));
    cxViewAppendAction(s1, m);
    cxViewAppend(l, s1);
    
    cxSprite s2 = cxSpriteCreateWithURL("texture.json?cucumber.png");
    cxViewSetPos(s2, cxVec2fv(300, 300));
    cxScale s = cxScaleCreate(5.0f, cxVec2fv(4, 4));
    cxViewAppendAction(s2, s);
    
    cxRotate r = cxRotateCreate(3.0f, cxVec3fZ, kmDegreesToRadians(360));
    cxViewAppendAction(s2, r);
    cxViewAppend(l, s2);
    
    cxWindowPushView(l);
}

void cxEngineFree(cxEngine engine)
{
    
}
