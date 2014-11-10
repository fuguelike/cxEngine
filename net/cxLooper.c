//
//  cxLooper.c
//  cxEngine
//
//  Created by xuhua on 11/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxLooper.h"

void cxLooperUpdate(cxAny loop)
{
    CX_ASSERT_THIS(loop, cxLooper);
    uv_run(&this->looper, UV_RUN_NOWAIT);
}
CX_TYPE(cxLooper, cxObject)
{
    
}
CX_INIT(cxLooper, cxObject)
{
    uv_loop_init(&this->looper);
}
CX_FREE(cxLooper, cxObject)
{
    uv_loop_close(&this->looper);
}
CX_TERM(cxLooper, cxObject)