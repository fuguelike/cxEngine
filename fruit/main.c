//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include "fcMap.h"

void cxEngineInit(cxEngine engine)
{
    
}

#include <views/cxButton.h>

cxButton eb;
cxButton fb;

static void cxFireModeClick(cxEvent *e)
{
    fcMap map = cxEventArgToWeakRef(e->args);
    fcMapSetMode(map, fcMapModeFire);
    cxViewSetAlpha(fb, 1.0f);
    cxViewSetAlpha(eb, 0.5f);
}

static void cxEditModeClick(cxEvent *e)
{
    fcMap map = cxEventArgToWeakRef(e->args);
    fcMapSetMode(map, fcMapModeEdit);
    cxViewSetAlpha(fb, 0.5f);
    cxViewSetAlpha(eb, 1.0f);
}

void cxEngineMain(cxEngine engine)
{
    fcMap map = CX_CREATE(fcMap);
    cxWindowPushView(map, NULL);
    
    eb = cxButtonCreate("item.xml?edit-mode.png");
    cxViewSetSize(eb, cxSize2fv(200, 150));
    cxViewSetAutoResizeMask(eb, cxViewAutoResizeLeft|cxViewAutoResizeBottom);
    CX_EVENT_APPEND(eb->onRelease, cxEditModeClick, cxEventArgWeakRef(map));
    cxViewAppend(engine->window, eb);
    
    fb = cxButtonCreate("item.xml?fire-mode.png");
    cxViewSetAlpha(fb, 0.5f);
    cxViewSetSize(fb, cxSize2fv(200, 150));
    cxViewSetAutoResizeMask(fb, cxViewAutoResizeRight|cxViewAutoResizeBottom);
    CX_EVENT_APPEND(fb->onRelease, cxFireModeClick, cxEventArgWeakRef(map));
    cxViewAppend(engine->window, fb);
}

void cxEngineFree(cxEngine engine)
{
    
}
