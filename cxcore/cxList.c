//
//  cxList.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxList.h"

CX_OBJECT_TYPE(cxList, cxObject)
{}
CX_OBJECT_INIT(cxList, cxObject)
{}
CX_OBJECT_FREE(cxList, cxObject)
{
    cxListClean(this);
}
CX_OBJECT_TERM(cxList, cxObject)

cxListElement *cxListAppend(cxAny plist,cxAny any)
{
    CX_ASSERT_THIS(plist, cxList);
    cxListElement *element = allocator->malloc(sizeof(cxListElement));
    element->any = any;
    DL_APPEND(this->listptr, element);
    CX_RETAIN(any);
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
    element->any = any;
    DL_PREPEND(this->listptr, element);
    CX_RETAIN(any);
    return element;
}

cxInt cxListLength(cxAny plist)
{
    CX_ASSERT_THIS(plist, cxList);
    cxInt count = 0;
    cxListElement *ele = NULL;
    DL_COUNT(this->listptr, ele, count);
    return count;
}

cxListElement *cxListFirst(cxAny plist)
{
    CX_ASSERT_THIS(plist, cxList);
    return this->listptr;
}

cxListElement *cxListToHead(cxAny plist,cxListElement *element)
{
    CX_ASSERT_THIS(plist, cxList);
    CX_RETURN(cxListFirst(plist) == element,element);
    DL_DELETE(this->listptr, element);
    DL_PREPEND(this->listptr, element);
    return element;
}

cxListElement *cxListToTail(cxAny plist,cxListElement *element)
{
    CX_ASSERT_THIS(plist, cxList);
    CX_RETURN(cxListLast(plist) == element,element);
    DL_DELETE(this->listptr, element);
    DL_APPEND(this->listptr, element);
    return element;
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
}

void cxListClean(cxAny plist)
{
    CX_ASSERT_THIS(plist, cxList);
    cxListElement *ele = NULL,*tmp=NULL;
    DL_FOREACH_SAFE(this->listptr,ele,tmp){
        cxListRemove(this, ele);
    }
}





