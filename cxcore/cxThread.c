//
//  cxThread.c
//  cxCore
//
//  Created by xuhua on 10/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxThread.h"

CX_DEF(cxThread, cxObject)
    pthread_t pid;
    pthread_attr_t attr;
    CX_METHOD_DEF(cxAny, Run, cxAny);
CX_END(cxThread, cxObject)

CX_TYPE(cxThread, cxObject)
{
    
}
CX_INIT(cxThread, cxObject)
{
    pthread_attr_init(&this->attr);
}
CX_FREE(cxThread, cxObject)
{
    pthread_attr_destroy(&this->attr);
}
CX_TERM(cxThread, cxObject)

static cxAny cxThreadRunImp(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxThread);
    cxMemPoolBegin();
    CX_METHOD_RUN(this->Run,this);
    cxMemPoolClear();
    CX_RELEASE(this);
    return NULL;
}

cxBool cxThreadRun(cxAny (*run)(cxAny))
{
    cxThread this = CX_ALLOC(cxThread);
    CX_METHOD_SET(this->Run, run);
    if(pthread_create(&this->pid, &this->attr, cxThreadRunImp, this) != 0){
        CX_ERROR("create thread failed");
        CX_RELEASE(this);
        return false;
    }
    return true;
}