//
//  cxList.h
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxList_h
#define cxCore_cxList_h

#include <utlist.h>
#include "cxCore.h"

CX_C_BEGIN

#define CX_LIST_FOREACH(_list_,_ele_)               \
cxListElement *_ele_ = NULL;                        \
DL_FOREACH((_list_)->listptr,_ele_)

#define CX_LIST_FOREACH_SAFE(_list_,_ele_,_tmp_)    \
cxListElement *_ele_ = NULL,*_tmp_=NULL;            \
DL_FOREACH_SAFE((_list_)->listptr,_ele_,_tmp_)

typedef struct cxListElement cxListElement;
struct cxListElement {
    cxListElement *prev, *next;
    cxAny any;
};

CX_DEF(cxList, cxObject)
    cxListElement *listptr;
    cxInt length;
CX_END(cxList, cxObject)

#define cxListSort(_list_,_cmp_)    DL_SORT((_list_)->listptr, _cmp_)

cxInt cxListLength(cxAny plist);

cxBool cxListEmpty(cxAny plist);

cxAny cxListTail(cxAny plist);

cxAny cxListHead(cxAny plist);

cxListElement *cxListAppend(cxAny plist,cxAny any);

cxListElement *cxListPrepend(cxAny plist,cxAny any);

cxListElement *cxListFirst(cxAny plist);

cxListElement *cxListMoveToHead(cxAny plist,cxListElement *element);

cxListElement *cxListMoveToTail(cxAny plist,cxListElement *element);

cxListElement *cxListLast(cxAny plist);

void cxListRemove(cxAny plist,cxListElement *element);

void cxListClear(cxAny plist);

CX_C_END

#endif
