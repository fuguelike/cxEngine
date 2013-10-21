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

void cxWindowPushView(cxAny pview)
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
    cxViewAppend(engine->window, new);
    cxEngineTimeReset();
}

void cxWindowPopView()
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
        cxViewEnter(top);
        cxViewSetVisible(top, true);
    }
    cxEngineTimeReset();
}

void cxWindowReplaceView(cxAny pview)
{
    CX_ASSERT(pview != NULL, "new view null");
    cxEngine engine = cxEngineInstance();
    cxView top = cxStackTop(engine->window->views);
    if(top != NULL){
        cxViewRemoved(top);
    }
    cxStackPush(engine->window->views, pview);
    cxViewAppend(engine->window, pview);
    cxEngineTimeReset();
}












