//
//  cxList.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxList.h"

/*
 items:[value1,value2]
*/

CX_SETTER_DEF(cxList, items)
{
    CX_ASSERT(cxJsonIsArray(value), "items must is array");
    cxJson items = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(items, v)
    cxAny any = cxJsonTocxObject(v);
    if(any != NULL){
        cxListAppend(this, any);
    }
    CX_JSON_ARRAY_EACH_END(items, v)
}

CX_TYPE(cxList, cxObject)
{
    CX_SETTER(cxList, items);
}
CX_INIT(cxList, cxObject)
{
    
}
CX_FREE(cxList, cxObject)
{
    cxListClear(this);
}
CX_TERM(cxList, cxObject)

cxListElement *cxListAppend(cxAny plist,cxAny any)
{
    CX_ASSERT_THIS(plist, cxList);
    cxListElement *element = allocator->malloc(sizeof(cxListElement));
    CX_RETAIN_SET(element->any, any);
    element->any = any;
    DL_APPEND(this->listptr, element);
    this->length++;
    return element;
}

cxBool cxListEmpty(cxAny plist)
{
    CX_ASSERT_THIS(plist, cxList);
    return this->listptr == NULL;
}

cxListElement *cxListPrepend(cxAny plist,cxAny any)
{
    CX_ASSERT_THIS(plist, cxList);
    cxListElement *element = allocator->malloc(sizeof(cxListElement));
    CX_RETAIN_SET(element->any, any);
    DL_PREPEND(this->listptr, element);
    this->length++;
    return element;
}

cxInt cxListLength(cxAny plist)
{
    CX_ASSERT_THIS(plist, cxList);
    return this->length;
}

cxListElement *cxListFirst(cxAny plist)
{
    CX_ASSERT_THIS(plist, cxList);
    return this->listptr;
}

cxListElement *cxListMoveToHead(cxAny plist,cxListElement *element)
{
    CX_ASSERT_THIS(plist, cxList);
    CX_RETURN(cxListFirst(plist) == element,element);
    DL_DELETE(this->listptr, element);
    DL_PREPEND(this->listptr, element);
    return element;
}

cxListElement *cxListMoveToTail(cxAny plist,cxListElement *element)
{
    CX_ASSERT_THIS(plist, cxList);
    CX_RETURN(cxListLast(plist) == element,element);
    DL_DELETE(this->listptr, element);
    DL_APPEND(this->listptr, element);
    return element;
}

cxAny cxListTail(cxAny plist)
{
    cxListElement *element = cxListLast(plist);
    return element != NULL ? element->any : NULL;
}

cxAny cxListHead(cxAny plist)
{
    cxListElement *element = cxListFirst(plist);
    return element != NULL ? element->any : NULL;
}

cxListElement *cxListLast(cxAny plist)
{
    CX_ASSERT_THIS(plist, cxList);
    return this->listptr == NULL ? NULL : this->listptr->prev;
}

void cxListRemove(cxAny plist,cxListElement *element)
{
    CX_ASSERT_THIS(plist, cxList);
    DL_DELETE(this->listptr, element);
    CX_RELEASE(element->any);
    allocator->free(element);
    this->length--;
}

void cxListClear(cxAny plist)
{
    CX_ASSERT_THIS(plist, cxList);
    cxListElement *ele = NULL,*tmp=NULL;
    DL_FOREACH_SAFE(this->listptr,ele,tmp){
        cxListRemove(this, ele);
    }
    CX_ASSERT(this->length == 0, "length not symmetry");
}





