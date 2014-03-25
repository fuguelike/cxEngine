//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>

void cxEngineInit(cxEngine engine)
{
    cxEngineLuaRunFile("main.lua");
    cxEngineLuaRunChars("init()");
}

#include <actions/cxFollow.h>
#include <actions/cxMove.h>
#include <views/cxSprite.h>
#include <views/cxButton.h>

//cxSprite sp1;
//cxSprite sp2;
//
//static void cxButtonClick1(cxEvent *e)
//{
//    cxFollow f = cxFollowCreate(1800, sp1);
//    cxViewAppendAction(sp2, f);
//    
//    cxMove m = cxMoveCreate(1.0f, cxVec2fv(600, 600));
//    cxViewAppendAction(sp1, m);
//}
//
//static void cxButtonClick2(cxEvent *e)
//{
//    cxViewSetPos(sp1, cxVec2fv(0, 300));
//    cxViewSetPos(sp2, cxVec2fv(0, -300));
//}

void cxEngineMain(cxEngine engine)
{
//    //cxWindowPushXML("main.xml");
//    sp1 = cxSpriteCreateWithURL("candy.xml?green.png");
//    cxViewSetPos(sp1, cxVec2fv(0, 300));
//    cxViewAppend(engine->window, sp1);
//    
//    sp2 = cxSpriteCreateWithURL("candy.xml?red.png");
//    cxViewSetPos(sp2, cxVec2fv(0, -300));
//    cxViewAppend(engine->window, sp2);
//    
//    {
//        cxButton btn = cxButtonCreate("scale9.png");
//        cxViewSetAutoResizeMask(btn, cxViewAutoResizeLeft|cxViewAutoResizeBottom);
//        CX_EVENT_APPEND(btn->onRelease, cxButtonClick1, NULL);
//        cxViewAppend(engine->window, btn);
//    }
//    {
//        cxButton btn = cxButtonCreate("scale9.png");
//        cxViewSetAutoResizeMask(btn, cxViewAutoResizeRight|cxViewAutoResizeBottom);
//        CX_EVENT_APPEND(btn->onRelease, cxButtonClick2, NULL);
//        cxViewAppend(engine->window, btn);
//    }
    cxEngineLuaRunChars("main()");
}

void cxEngineFree(cxEngine engine)
{
    cxEngineLuaRunChars("free()");
}
