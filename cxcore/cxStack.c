//
//  cxStack.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxJson.h"
#include "cxStack.h"

/*
 items:[value1,value2]
*/

CX_SETTER_DEF(cxStack, items)
{
    CX_ASSERT(cxJsonIsArray(value), "items must is array");
    cxJson items = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(items, v)
    cxAny any = cxJsonTocxObject(v);
    if(any != NULL){
        cxStackPush(this, any);
    }
    CX_JSON_ARRAY_EACH_END(items, v)
}

CX_TYPE(cxStack, cxObject)
{
    CX_SETTER(cxStack, items);
}
CX_INIT(cxStack, cxObject)
{
    this->array = CX_ALLOC(cxArray);
}
CX_FREE(cxStack, cxObject)
{
    CX_RELEASE(this->array);
}
CX_TERM(cxStack, cxObject)

void cxStackClear(cxAny pstack)
{
    CX_ASSERT_THIS(pstack, cxStack);
    cxArrayClear(this->array);
}

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

void cxStackRemove(cxAny pstack,cxAny pobj)
{
    CX_ASSERT_THIS(pstack, cxStack);
    cxArrayRemove(this->array, pobj);
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