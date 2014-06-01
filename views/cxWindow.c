//
//  cxWindow.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxWindow.h"

static cxBool cxWindowOnKey(cxAny pview,cxKey *key)
{
    if(key->code == CX_KEYCODE_BACK && key->type == cxKeyTypeUp){
        cxEngineExit();
    }
    return true;
}

CX_OBJECT_TYPE(cxWindow, cxView)
{
    
}
CX_OBJECT_INIT(cxWindow, cxView)
{
    CX_METHOD_SET(this->cxView.OnKey, cxWindowOnKey);
    this->views = CX_ALLOC(cxStack);
}
CX_OBJECT_FREE(cxWindow, cxView)
{
    CX_RELEASE(this->views);
}
CX_OBJECT_TERM(cxWindow, cxView)

cxAny cxWindowTopView()
{
    cxEngine engine = cxEngineInstance();
    return cxStackTop(engine->window->views);
}

void cxWindowPushView(cxAny pview)
{
    CX_ASSERT(CX_INSTANCE_OF(pview, cxView), "pview type error");
    cxEngine engine = cxEngineInstance();
    cxView new = pview;
    cxView top = cxStackTop(engine->window->views);
    if(top != NULL && new->hideTop){
        cxViewSetVisible(top, false);
        cxViewExit(top);
    }
    cxStackPush(engine->window->views, new);
    cxViewAppend(engine->window, new);
}

void cxWindowPopView()
{
    cxEngine engine = cxEngineInstance();
    cxBool hideTop = false;
    cxView prev = cxStackTop(engine->window->views);
    cxStackPop(engine->window->views);
    cxView top = cxStackTop(engine->window->views);
    if(prev != NULL){
        hideTop = prev->hideTop;
        cxViewRemoved(prev);
    }
    if(top != NULL && hideTop){
        cxViewEnter(top);
        cxViewSetVisible(top, true);
    }
}

void cxWindowReplaceView(cxAny pview)
{
    CX_ASSERT(CX_INSTANCE_OF(pview, cxView), "pview type error");
    cxEngine engine = cxEngineInstance();
    cxView top = cxStackTop(engine->window->views);
    if(top != NULL){
        cxViewRemoved(top);
        cxStackPop(engine->window->views);
    }
    cxStackPush(engine->window->views, pview);
    cxViewAppend(engine->window, pview);
}












