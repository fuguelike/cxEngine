//
//  cxStack.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxStack.h"

void __cxStackInitObject(cxAny object,cxAny json,cxAny hash)
{
    //load items
    CX_OBJECT_INIT_SUPER(cxObject);
}

CX_OBJECT_INIT(cxStack, cxObject)
{
    CX_OBJECT_INIT_OVERRIDE(cxStack);
    this->array = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxStack, cxObject)
{
    CX_RELEASE(this->array);
}
CX_OBJECT_TERM(cxStack, cxObject)

void cxStackPush(cxStack stack,cxAny any)
{
    cxArrayAppend(stack->array, any);
}

cxAny cxStackTop(cxStack stack)
{
    return cxArrayLast(stack->array);
}

cxInt cxStackLength(cxStack stack)
{
    return cxArrayLength(stack->array);
}

void cxStackReplaceTop(cxStack stack,cxAny any)
{
    cxInt last = cxArrayLength(stack->array) - 1;
    if(last < 0){
        cxStackPush(stack, any);
    }else{
        cxArrayUpdate(stack->array, any, last);
    }
}

void cxStackPop(cxStack stack)
{
    CX_RETURN(cxArrayLength(stack->array) == 0);
    cxArrayRemoveLast(stack->array);
}

cxAny cxStackObject(cxStack stack,cxIndex index)
{
    return cxArrayAtIndex(stack->array, index);
}