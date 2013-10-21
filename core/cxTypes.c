//
//  cxTypes.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxHash.h"
#include "cxTypes.h"

static const UT_icd cxBoxPointICD = {sizeof(cxBoxPoint),NULL,NULL,NULL};

CX_OBJECT_INIT(cxTypes, cxObject)
{
    this->kvs = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxTypes, cxObject)
{
    CX_RELEASE(this->assist);
    CX_RELEASE(this->kvs);
    utarray_free(this->utArray);
}
CX_OBJECT_TERM(cxType, cxObject)

cxTypes cxBoxPointTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    utarray_new(this->utArray, &cxBoxPointICD);
    return this;
}

cxAny cxTypesValue(cxTypes this,cxConstChars key)
{
    return cxHashGet(this->kvs, cxHashStrKey(key));
}

void cxTypesAppendKeyValue(cxTypes this,cxConstChars key,cxAny value)
{
    CX_ASSERT(key != NULL && value != NULL, "key value null");
    cxHashSet(this->kvs, cxHashStrKey(key), value);
}

cxVec2f cxCardinalSplineAt(cxVec2f p0, cxVec2f p1, cxVec2f p2, cxVec2f p3, cxFloat tension, cxFloat t)
{
    cxFloat t2 = t * t;
    cxFloat t3 = t2 * t;
    cxFloat s = (1 - tension) / 2;
    cxFloat b1 = s * ((-t3 + (2 * t2)) - t);
    cxFloat b2 = s * (-t3 + t2) + (2 * t3 - 3 * t2 + 1);
    cxFloat b3 = s * (t3 - 2 * t2 + t) + (-2 * t3 + 3 * t2);
    cxFloat b4 = s * (t3 - t2);
    cxFloat x = (p0.x*b1 + p1.x*b2 + p2.x*b3 + p3.x*b4);
    cxFloat y = (p0.y*b1 + p1.y*b2 + p2.y*b3 + p3.y*b4);
	return cxVec2fv(x, y);
}

cxRect4f cxBoxTex2fToRect4f(cxBoxTex2f box)
{
    cxFloat x = box.lt.u;
    cxFloat y = box.lt.v;
    cxFloat w = box.rt.u - box.lt.u;
    cxFloat h = box.rb.v - box.rt.v;
    return cxRect4fv(x, y, w, h);
}

cxBoxTex2f cxRect4fToBoxTex2f(cxRect4f rect)
{
    cxBoxTex2f box;
    box.lb = cxTex2fv(rect.x, rect.y + rect.h);
    box.lt = cxTex2fv(rect.x, rect.y);
    box.rb = cxTex2fv(rect.x + rect.w, rect.y + rect.h);
    box.rt = cxTex2fv(rect.x + rect.w, rect.y);
    return box;
}

cxBool cxBox2fContainPoint(const cxBoxVec2f box,const cxVec2f pos)
{
    return (pos.x >= box.lb.x && pos.x <= box.rt.x && pos.y >= box.lb.y && pos.y <= box.rt.y);
}

cxBool cxPolygonContainPoint(const cxPolygon *polygon,const cxVec2f tp)
{
    cxBool c = false;
    cxInt num = polygon->num;
    const cxVec2f *vs = polygon->vs;
    for (int i = 0,j = num-1; i < num; j = i++) {
        if(!((vs[i].y > tp.y) != (vs[j].y>tp.y))){
            continue;
        }
        if(tp.x < (vs[j].x - vs[i].x) * (tp.y - vs[i].y) / (vs[j].y - vs[i].y) + vs[i].x){
            c = !c;
        }
    }
    return c;
}

cxFloat cxBezier2(cxFloat a, cxFloat b, cxFloat c, cxFloat t)
{
    return (powf(1-t,2) * a +2*t*(1-t)*b +powf(t,2)*c);
}

cxFloat cxBezier3(cxFloat a, cxFloat b, cxFloat c, cxFloat d, cxFloat t)
{
    return (powf(1-t,3) * a +3*t*(powf(1-t,2))*b +3*powf(t,2)*(1-t)*c +powf(t,3)*d);
}