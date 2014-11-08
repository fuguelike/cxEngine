//
//  cxSpatial.c
//  Engine
//
//  Created by xuhua on 9/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//


#include "cxView.h"
#include "cxSpatial.h"

static void cxSpatialIndexIterator(void *obj, void *data)
{
    CX_ASSERT_THIS(data, cxSpatial);
    CX_ASSERT_VALUE(obj, cxView, view);
    cpHashValue hv = (cpHashValue)CX_CALL(this, HashValue, CX_MT(cxUInt,cxAny), view);
    cpSpatialIndexRemove(this->index, view, hv);
    CX_RELEASE(view);
}

static cxUInt CX_METHOD(cxSpatial, HashValue,cxAny pview)
{
    return (cxUInt)pview;
}

static cpBB CX_METHOD(cxSpatial, IndexBB)
{
    cxBox4f box = cxViewGetBox(this);
    cxVec2f pos = cxViewGetPosition(this);
    return cpBBNew(box.l + pos.x, box.b + pos.y, box.r + pos.x, box.t + pos.y);
}

CX_TYPE(cxSpatial, cxObject)
{
    CX_MSET(cxSpatial, HashValue);
    CX_MSET(cxSpatial, IndexBB);
}
CX_INIT(cxSpatial, cxObject)
{
    cpSpatialIndexBBFunc bbFunc = CX_MGET(this, IndexBB);
    this->index = cpBBTreeNew(bbFunc, NULL);
}
CX_FREE(cxSpatial, cxObject)
{
    if(this->index != NULL){
        cxSpatialClear(this);
        cpSpatialIndexDestroy(this->index);
    }
}
CX_TERM(cxSpatial, cxObject)

cxSpatial cxSpatialAlloc(cpBB (*func)(cxAny))
{
    cxSpatial this = CX_ALLOC(cxSpatial);
    if(this->index != NULL){
        cxSpatialClear(this);
        cpSpatialIndexDestroy(this->index);
    }
    this->index = cpBBTreeNew(func, NULL);
    return this;
}

cxSpatial cxSpatialCreate(cpBB (*func)(cxAny))
{
    cxSpatial this = cxSpatialAlloc(func);
    return CX_AUTO(this);
}

typedef struct {
    cxAny ps;
    cxAny data;
    cxSpatialEachFunc func;
}cxSpatialEachInfo;

static void cxSpatialEachIterator(void *obj, void *data)
{
    CX_ASSERT_TYPE(obj, cxView);
    cxSpatialEachInfo *info = data;
    info->func(info->ps,obj,info->data);
}

void cxSpatialEach(cxAny ps,cxSpatialEachFunc func,cxAny data)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT(func != NULL, "args error");
    cxSpatialEachInfo info = {0};
    info.data = data;
    info.ps = ps;
    info.func = func;
    cpSpatialIndexEach(this->index, cxSpatialEachIterator, &info);
}

static cpCollisionID cxSpatialIndexQueryFunc(cxAny ps, cxAny pview, cpCollisionID id, void *data)
{
    cxSpatialNearestInfo *info = data;
    CX_ASSERT_VALUE(pview,cxView,view);
    if(info->filter != NULL && !info->filter(view)){
        return id;
    }
    cxVec2f vpos = cxViewGetPosition(view);
    cxFloat d = kmVec2DistanceBetween(&info->p, &vpos);
    if(d < info->d){
        info->view = view;
        info->d = d;
    }
    return id;
}

cxSpatialNearestInfo cxSpatialNearest(cxAny ps,cxVec2f p,cxFloat max,cxSpatialNearestFilter filter)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    cxSpatialNearestInfo ret = {0};
    ret.p = p;
    ret.d = INT32_MAX;
    ret.view = NULL;
    ret.m = max;
    ret.filter = filter;
    cpBB bb = cpBBNewForCircle(p, cpfmax(max, 0.0f));
    cpSpatialIndexQuery(this->index, this, bb, cxSpatialIndexQueryFunc, &ret);
    return ret;
}

void cxSpatialClear(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    cpSpatialIndexEach(this->index, cxSpatialIndexIterator, this);
}

void cxSpatialInsert(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    cpHashValue hv = (cpHashValue)CX_CALL(this, HashValue, CX_MT(cxUInt,cxAny), view);
    cpSpatialIndexInsert(this->index, view, hv);
    CX_RETAIN(view);
}

void cxSpatialRemove(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    if(!cxSpatialContains(ps, view)){
        return;
    }
    cpHashValue hv = (cpHashValue)CX_CALL(this, HashValue, CX_MT(cxUInt,cxAny), view);
    cpSpatialIndexRemove(this->index, view, hv);
    CX_RELEASE(view);
}

cxBool cxSpatialContains(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    cpHashValue hv = (cpHashValue)CX_CALL(this, HashValue, CX_MT(cxUInt,cxAny), view);
    return cpSpatialIndexContains(this->index, view, hv);
}

void cxSpatialReindex(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    cpSpatialIndexReindex(this->index);
}

void cxSpatialReindexView(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    cpHashValue hv = (cpHashValue)CX_CALL(this, HashValue, CX_MT(cxUInt,cxAny), view);
    cpSpatialIndexReindexObject(this->index, view, hv);
}

cxInt cxSpatialCount(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    return cpSpatialIndexCount(this->index);
}






