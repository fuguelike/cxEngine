//
//  cxList.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

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

cxListElement *cxListAppend(cxList list,cxAny any)
{
    cxListElement *element = allocator->malloc(sizeof(cxListElement));
    element->any = any;
    DL_APPEND(list->listptr, element);
    CX_RETAIN(any);
    return element;
}

cxBool cxListEmpty(cxList list)
{
    return list->listptr == NULL;
}

cxListElement *cxListPrepend(cxList list,cxAny any)
{
    cxListElement *element = allocator->malloc(sizeof(cxListElement));
    element->any = any;
    DL_PREPEND(list->listptr, element);
    CX_RETAIN(any);
    return element;
}

cxInt cxListLength(cxList list)
{
    cxInt count = 0;
    cxListElement *ele = NULL;
    DL_COUNT(list->listptr, ele, count);
    return count;
}

cxListElement *cxListFirst(cxList list)
{
    return list->listptr;
}

cxListElement *cxListLast(cxList list)
{
    return list->listptr == NULL ? NULL : list->listptr->prev;
}

void cxListRemove(cxList list,cxListElement *element)
{
    DL_DELETE(list->listptr, element);
    CX_RELEASE(element->any);
    allocator->free(element);
}

void cxListClean(cxList list)
{
    cxListElement *ele = NULL,*tmp=NULL;
    DL_FOREACH_SAFE(list->listptr,ele,tmp){
        cxListRemove(list, ele);
    }
}





