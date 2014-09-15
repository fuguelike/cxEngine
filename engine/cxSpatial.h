//
//  cxSpatial.h
//  Engine
//  cxView spatial index
//  Created by xuhua on 9/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSpatial_h
#define cxEngine_cxSpatial_h

#include <cxcore/cxCore.h>

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

CX_OBJECT_DEF(cxSpatial, cxObject)
    cxAny index;
    CX_METHOD_DEF(cxUInt, HashValue, cxAny);
CX_OBJECT_END(cxSpatial, cxObject)

void cxSpatialEach(cxAny ps,cxSpatialEachFunc func,cxAny data);

void cxSpatialClean(cxAny ps);

void cxSpatialInsert(cxAny ps,cxAny view);

void cxSpatialRemove(cxAny ps,cxAny view);

cxBool cxSpatialContains(cxAny ps,cxAny view);

void cxSpatialReindex(cxAny ps);

void cxSpatialReindexView(cxAny ps,cxAny view);

cxInt cxSpatialCount(cxAny ps);

cxSpatialNearestInfo cxSpatialNearest(cxAny ps,cxVec2f p,cxFloat max,cxSpatialNearestFilter filter);

CX_C_END

#endif
