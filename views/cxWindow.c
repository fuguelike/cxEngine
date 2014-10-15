//
//  cxWindow.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxWindow.h"

static cxBool cxWindowKey(cxAny pview,const cxKey *key)
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
    CX_SET(cxView, this, Key, cxWindowKey);
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
    return cxStackTop(engine->Window->views);
}

void cxWindowPushView(cxAny pview)
{
    CX_ASSERT_TYPE(pview, cxView);
    cxEngine engine = cxEngineInstance();
    cxView new = pview;
    cxView top = cxWindowTopView();
    if(top != NULL && cxViewGetSleepTop(new)){
        cxViewSetIsSleep(top, true);
        cxViewExit(top);
    }
    cxStackPush(engine->Window->views, new);
    cxViewAppend(engine->Window, new);
}

void cxWindowPopView()
{
    cxEngine engine = cxEngineInstance();
    cxBool hideTop = false;
    cxView prev = cxWindowTopView();
    cxStackPop(engine->Window->views);
    cxView top = cxWindowTopView();
    if(prev != NULL){
        hideTop = cxViewGetSleepTop(prev);
        cxViewRemove(prev);
    }
    if(top != NULL && hideTop){
        cxViewEnter(top);
        cxViewSetIsSleep(top, false);
    }
}

void cxWindowReplaceView(cxAny pview)
{
    CX_ASSERT_TYPE(pview, cxView);
    cxWindowPopView();
    cxWindowPushView(pview);
}












