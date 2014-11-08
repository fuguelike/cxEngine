//
//  cxRunner.c
//  cxEngine
//
//  Created by xuhua on 11/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxRunner.h"


static cxBool CX_METHOD(cxRunner,Exit)
{
    if(this->count == 0){
        this->step ++;
    }
    return this->count == 0;
}

CX_TYPE(cxRunner, cxAction)
{
    CX_MSET(cxRunner, Exit);
}
CX_INIT(cxRunner, cxAction)
{
    
}
CX_FREE(cxRunner, cxAction)
{
    //
}
CX_TERM(cxRunner, cxAction)

static void cxRunnerItemStop(cxAny sender)
{
    CX_ASSERT_TYPE(sender, cxAction);
    cxAny parent = cxActionGetParent(sender);
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
    CX_ADD(cxAction, action, onExit, cxRunnerItemStop);
    cxViewAppendAction(pview, pav);
    cxActionSetParent(pav, runner);
}

