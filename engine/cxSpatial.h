//
//  cxSpatial.h
//  Engine
//  cxView spatial index
//  Created by xuhua on 9/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSpatial_h
#define cxEngine_cxSpatial_h

#include <chipmunk/chipmunk.h>
#include <cxcore/cxBase.h>

CX_C_BEGIN

typedef cxBool (*cxSpatialNearestFilter)(cxAny pview);

typedef void (*cxSpatialEachFunc)(cxAny ps,cxAny pview,cxAny data);

typedef struct {
    cxAny view;     //最近的view
    cxVec2f p;      //与此点的
    cxFloat d;      //距离
    cxFloat m;      //max
    cxSpatialNearestFilter filter;
} cxSpatialNearestInfo;

CX_DEF(cxSpatial, cxObject)
    CX_FIELD_DEF(cxAny Index);
CX_END(cxSpatial, cxObject)

CX_FIELD_GET(cxSpatial, cxAny, Index);

void cxSpatialEach(cxAny ps,cxSpatialEachFunc func,cxAny data);

void cxSpatialClear(cxAny ps);

void cxSpatialInsert(cxAny ps,cxAny view);

void cxSpatialRemove(cxAny ps,cxAny view);

cxBool cxSpatialContains(cxAny ps,cxAny view);

void cxSpatialReindex(cxAny ps);

void cxSpatialReindexView(cxAny ps,cxAny view);

cxInt cxSpatialCount(cxAny ps);

void cxSpatialIndexSegmentQuery(cxAny ps,cxAny obj, cpVect a, cpVect b, cpFloat t_exit, cpSpatialIndexSegmentQueryFunc func, cxAny data);

void cxSpatialIndexQuery(cxAny ps,cxAny obj, cpBB bb, cpSpatialIndexQueryFunc func, cxAny data);

cxSpatialNearestInfo cxSpatialNearest(cxAny ps,cxVec2f p,cxFloat max,cxSpatialNearestFilter filter);

CX_C_END

#endif
