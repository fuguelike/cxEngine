//
//  cxStack.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxStack.h"

CX_OBJECT_TYPE(cxStack, cxObject)
{}
CX_OBJECT_INIT(cxStack, cxObject)
{
    this->array = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxStack, cxObject)
{
    CX_RELEASE(this->array);
}
CX_OBJECT_TERM(cxStack, cxObject)

void cxStackPush(cxAny pstack,cxAny any)
{
    CX_ASSERT_THIS(pstack, cxStack);
    cxArrayAppend(this->array, any);
}

cxAny cxStackTop(cxAny pstack)
{
    CX_ASSERT_THIS(pstack, cxStack);
    return cxArrayLast(this->array);
}

cxInt cxStackLength(cxAny pstack)
{
    CX_ASSERT_THIS(pstack, cxStack);
    return cxArrayLength(this->array);
}

void cxStackReplaceTop(cxAny pstack,cxAny any)
{
    CX_ASSERT_THIS(pstack, cxStack);
    cxInt last = cxArrayLength(this->array) - 1;
    if(last < 0){
        cxStackPush(this, any);
    }else{
        cxArrayUpdate(this->array, any, last);
    }
}

void cxStackPop(cxAny pstack)
{
    CX_ASSERT_THIS(pstack, cxStack);
    CX_RETURN(cxArrayLength(this->array) == 0);
    cxArrayRemoveLast(this->array);
}

cxAny cxStackObject(cxAny pstack,cxInt index)
{
    CX_ASSERT_THIS(pstack, cxStack);
    return cxArrayAtIndex(this->array, index);
}