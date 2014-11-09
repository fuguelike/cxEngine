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
    cpHashValue hv = CX_CALL(this, HashValue, CX_M(cpHashValue,cxAny), view);
    cpSpatialIndexRemove(this->Index, view, hv);
    CX_RELEASE(view);
}

CX_METHOD_DEF(cxSpatial, HashValue,cpHashValue,cxAny pview)
{
    return (cpHashValue)pview;
}
CX_METHOD_DEF(cxView,IndexBB,cpBB)
{
    cxBox4f box = cxViewGetBox(this);
    cxVec2f pos = cxViewGetPosition(this);
    return cpBBNew(box.l + pos.x, box.b + pos.y, box.r + pos.x, box.t + pos.y);
}
CX_TYPE(cxSpatial, cxObject)
{
    CX_METHOD(cxSpatial, HashValue);
    CX_METHOD(cxView, IndexBB);
}
CX_INIT(cxSpatial, cxObject)
{
    cpSpatialIndexBBFunc bbFunc = CX_METHOD_GET(this, IndexBB);
    CX_ASSERT(bbFunc != NULL, "not define bbFunc method");
    this->Index = cpBBTreeNew(bbFunc, NULL);
}
CX_FREE(cxSpatial, cxObject)
{
    if(this->Index != NULL){
        cxSpatialClear(this);
        cpSpatialIndexDestroy(this->Index);
    }
}
CX_TERM(cxSpatial, cxObject)

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
    cpSpatialIndexEach(this->Index, cxSpatialEachIterator, &info);
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
    cpSpatialIndexQuery(this->Index, this, bb, cxSpatialIndexQueryFunc, &ret);
    return ret;
}

void cxSpatialClear(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    cpSpatialIndexEach(this->Index, cxSpatialIndexIterator, this);
}

void cxSpatialInsert(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    cpHashValue hv = CX_CALL(this, HashValue, CX_M(cpHashValue,cxAny), view);
    cpSpatialIndexInsert(this->Index, view, hv);
    CX_RETAIN(view);
}

void cxSpatialRemove(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    if(!cxSpatialContains(ps, view)){
        return;
    }
    cpHashValue hv = CX_CALL(this, HashValue, CX_M(cpHashValue,cxAny), view);
    cpSpatialIndexRemove(this->Index, view, hv);
    CX_RELEASE(view);
}

cxBool cxSpatialContains(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    cpHashValue hv = CX_CALL(this, HashValue, CX_M(cpHashValue,cxAny), view);
    return cpSpatialIndexContains(this->Index, view, hv);
}

void cxSpatialReindex(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    cpSpatialIndexReindex(this->Index);
}

void cxSpatialReindexView(cxAny ps,cxAny view)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    CX_ASSERT_TYPE(view, cxView);
    cpHashValue hv = CX_CALL(this, HashValue, CX_M(cpHashValue,cxAny), view);
    cpSpatialIndexReindexObject(this->Index, view, hv);
}

cxInt cxSpatialCount(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    return cpSpatialIndexCount(this->Index);
}






