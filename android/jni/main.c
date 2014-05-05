//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <actions/cxTimer.h>

void cxEngineInit(cxEngine engine)
{
    
    
}

static void timerArrive(cxEvent *e)
{
    cxEngineSendJson(UTF8("{\"json\":true}"));
}

void cxEngineMain(cxEngine engine)
{
    cxTimer timer = cxViewAppendTimer(engine->window, 1.0f, CX_TIMER_FOREVER);
    CX_EVENT_QUICK(timer->onArrive, timerArrive);
}

void cxEngineFree(cxEngine engine)
{
    
}
