//
//  cxRunner.c
//  cxEngine
//
//  Created by xuhua on 11/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxRunner.h"

static void cxRunnerInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxRunner);
    CX_METHOD_RUN(this->Init, this);
}

static cxBool cxRunnerExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxRunner);
    if(this->count == 0){
        this->step ++;
        CX_METHOD_RUN(this->Exit, this);
    }
    return this->count == 0;
}


CX_OBJECT_TYPE(cxRunner, cxAction)
{
    
}
CX_OBJECT_INIT(cxRunner, cxAction)
{
    CX_METHOD_SET(this->cxAction.Init, cxRunnerInit);
    CX_METHOD_SET(this->cxAction.Exit, cxRunnerExit);
}
CX_OBJECT_FREE(cxRunner, cxAction)
{
    //
}
CX_OBJECT_TERM(cxRunner, cxAction)

static void cxRunnerItemStop(cxAny sender)
{
    CX_ASSERT_TYPE(sender, cxAction);
    cxAny parent = cxActionParent(sender);
    CX_ASSERT_THIS(parent, cxRunner);
    CX_ASSERT(this != NULL, "parent null");
    this->count --;
}

cxRunner cxRunnerCreate(cxAny data)
{
    cxRunner this = CX_CREATE(cxRunner);
    this->data = data;
    return this;
}

void cxRunnerAppend(cxAny runner,cxAny pav, cxAny pview)
{
    CX_ASSERT_THIS(runner, cxRunner);
    CX_ASSERT_TYPE(pav, cxAction);
    CX_ASSERT_TYPE(pview, cxView);
    cxAction action = pav;
    this->count ++;
    CX_EVENT_APPEND(action->onExit, cxRunnerItemStop);
    cxViewAppendAction(pview, pav);
    cxActionSetParent(pav, runner);
}

