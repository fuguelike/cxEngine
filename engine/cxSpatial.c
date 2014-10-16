//
//  cxSpatial.c
//  Engine
//
//  Created by xuhua on 9/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//


#include "cxView.h"
#include "cxSpatial.h"

static cpBB cxSpatialIndexBB(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxBox4f box = cxViewGetBox(this);
    cxVec2f pos = cxViewGetPosition(this);
    return cpBBNew(box.l + pos.x, box.b + pos.y, box.r + pos.x, box.t + pos.y);
}

static void cxSpatialIndexIterator(void *obj, void *data)
{
    CX_ASSERT_THIS(data, cxSpatial);
    CX_ASSERT_VALUE(obj, cxView, view);
    cpHashValue hv = CX_METHOD_GET((cpHashValue)view, this->HashValue, view);
    cpSpatialIndexRemove(this->index, view, hv);
    CX_RELEASE(view);
}

CX_OBJECT_TYPE(cxSpatial, cxObject)
{
    
}
CX_OBJECT_INIT(cxSpatial, cxObject)
{
    CX_SET(cxSpatial, this, IndexBB, cxSpatialIndexBB);
    this->index = cpBBTreeNew(this->IndexBB, NULL);
}
CX_OBJECT_FREE(cxSpatial, cxObject)
{
    if(this->index != NULL){
        cxSpatialClear(this);
        cpSpatialIndexDestroy(this->index);
    }
}
CX_OBJECT_TERM(cxSpatial, cxObject)

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
    cpHashValue hv = CX_METHOD_GET((cpHashValue)view, this->HashValue, view);
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
    cpHashValue hv = CX_METHOD_GET((cpHashValue)view, this->HashValue, view);
    cpSpatialIndexRemove(this->index, view, hv);
    CX_RELEASE(view);
}

cxBool cxSpatialContains(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    cpHashValue hv = CX_METHOD_GET((cpHashValue)view, this->HashValue, view);
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
    cpHashValue hv = CX_METHOD_GET((cpHashValue)view, this->HashValue, view);
    cpSpatialIndexReindexObject(this->index, view, hv);
}

cxInt cxSpatialCount(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    return cpSpatialIndexCount(this->index);
}






