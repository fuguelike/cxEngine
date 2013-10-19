//
//  cxList.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxList_h
#define cxEngine_cxList_h

#include <utlist.h>
#include "cxBase.h"


CX_C_BEGIN

#define CX_LIST_FOREACH(list,ele)   cxListElement *ele = NULL;DL_FOREACH((list)->listptr,ele)

#define CX_LIST_SAFE_FOREACH(list,ele,tmp)  cxListElement *ele = NULL,*tmp=NULL;DL_FOREACH_SAFE((list)->listptr,ele,tmp)

typedef struct cxListElement cxListElement;
struct cxListElement {
    cxListElement *prev, *next;
    cxAny object;
};

typedef int (*cxListCmpFunc)(cxListElement *lp,cxListElement *rp);

CX_OBJECT_DEF(cxList, cxObject)
    cxListElement *listptr;
CX_OBJECT_END(cxList)

#define cxListSort(list,cmp)    DL_SORT((list)->listptr, cmp)

cxInt cxListLength(cxList list);

cxListElement *cxListAppend(cxList list,cxAny any);

cxListElement *cxListPrepend(cxList list,cxAny any);

cxListElement *cxListFirst(cxList list);

cxListElement *cxListLast(cxList list);

void cxListRemove(cxList list,cxListElement *element);

void cxListClean(cxList list);

CX_C_END

#endif
