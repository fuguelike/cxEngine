//
//  cxTimeLine.h
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTimeLine_h
#define cxEngine_cxTimeLine_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxTimeLine, cxAction)
    CX_FIELD_DEF(cxInt Index);
    cxAnyArray times;
CX_END(cxTimeLine, cxAction)

CX_FIELD_GET(cxTimeLine, cxInt, Index);

//get time count
CX_INLINE cxInt cxTimeLineRemain(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxTimeLine);
    return cxAnyArrayLength(this->times) - this->Index - 1;
}

CX_INLINE cxInt cxTimeLineNumber(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxTimeLine);
    return cxAnyArrayLength(this->times);
}

//get time
cxFloat cxTimeLineGetTime(cxAny pav,cxInt idx);

//set time event point
void cxTimeLineSet(cxAny pav,cxFloat time);

//add time ,lasttime + add
void cxTimeLineAdd(cxAny pav,cxFloat add);

CX_C_END

#endif
