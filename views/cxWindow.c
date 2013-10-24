//
//  cxWindow.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxWindow.h"

CX_OBJECT_INIT(cxWindow, cxView)
{
    this->views = CX_ALLOC(cxStack);
}
CX_OBJECT_FREE(cxWindow, cxView)
{
    CX_RELEASE(this->views);
}
CX_OBJECT_TERM(cxWindow, cxView)

void cxWindowPushView(cxAny pview,cxAny args)
{
    CX_ASSERT(pview != NULL, "new view null");
    cxEngine engine = cxEngineInstance();
    cxView new = pview;
    cxView top = cxStackTop(engine->window->views);
    if(top != NULL && new->isTop){
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
    cxBool isTop = false;
    cxView prev = cxStackTop(engine->window->views);
    cxStackPop(engine->window->views);
    cxView top = cxStackTop(engine->window->views);
    if(prev != NULL){
        isTop = prev->isTop;
        cxViewRemoved(prev);
    }
    if(top != NULL && isTop){
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












