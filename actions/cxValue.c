//
//  cxValue.c
//  cxEngine
//
//  Created by xuhua on 3/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxValue.h"

CX_METHOD_DEF(cxValue,Init,void)
{
    for(cxInt i=0;i<this->count;i++){
        CX_ASSERT_POINTER(cxActionGetView(this), this->ptr[i]);
        this->delta[i] = this->to[i] - this->from[i];
    }
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxValue,Step,void,cxFloat dt,cxFloat time)
{
    for(cxInt i=0;i<this->count;i++){
        *(this->ptr[i]) = this->from[i] + time * this->delta[i];
    }
    CX_CALL(this, OnValue, CX_M(void));
}
CX_METHOD_DEF(cxValue, OnValue, void)
{
    
}
CX_TYPE(cxValue, cxAction)
{
    CX_METHOD(cxValue, Init);
    CX_METHOD(cxValue, Step);
    CX_METHOD(cxValue, OnValue);
}
CX_INIT(cxValue, cxAction)
{
    
}
CX_FREE(cxValue, cxAction)
{
    
}
CX_TERM(cxValue, cxAction)

cxValue cxValueCreateVec2f(cxFloat time,cxVec2f *ptr,cxVec2f to)
{
    cxValue this = CX_CREATE(cxValue);
    //x
    this->ptr[0] = &ptr->x;
    this->from[0] = ptr->x;
    this->to[0] = to.x;
    //y
    this->ptr[1] = &ptr->y;
    this->from[1] = ptr->y;
    this->to[1] = to.y;
    //count
    this->count = 2;
    cxActionSetTime(this, time);
    return this;
}

cxValue cxValueCreateFloat(cxFloat time,cxFloat *ptr,cxFloat to)
{
    cxValue this = CX_CREATE(cxValue);
    this->ptr[0] = ptr;
    this->from[0] = *ptr;
    this->to[0] = to;
    this->count = 1;
    cxActionSetTime(this, time);
    return this;
}