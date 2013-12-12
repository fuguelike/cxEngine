//
//  cxTypes.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxHash.h"
#include "cxTypes.h"
#include "cxArray.h"

static const UT_icd cxAtlasBoxPointTypeICD = {sizeof(cxAtlasBoxPointType),NULL,NULL,NULL};

CX_OBJECT_INIT(cxTypes, cxObject)
{
    this->kvs = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxTypes, cxObject)
{
    CX_RELEASE(this->assist);
    CX_RELEASE(this->kvs);
    if(this->utArray != NULL){
        utarray_free(this->utArray);
    }
}
CX_OBJECT_TERM(cxType, cxObject)

cxTypes cxAtlasBoxPointTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesAtlasBoxPoint;
    utarray_new(this->utArray, &cxAtlasBoxPointTypeICD);
    return this;
}

cxTypes cxStringTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesString;
    return this;
}

cxTypes cxHashTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesHash;
    return this;
}

cxTypes cxNumberTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesNumber;
    return this;
}

cxTypes cxArrayTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesArray;
    this->assist = CX_ALLOC(cxArray);
    return this;
}

cxTypes cxDBTypesCreate(cxAny db)
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesDB;
    CX_RETAIN_SWAP(this->assist, db);
    return this;
}

void cxTypesSet(cxTypes this,cxConstChars key,cxAny value)
{
    cxHashSet(this->kvs, cxHashStrKey(key), value);
}

cxAny cxTypesGet(cxTypes this,cxConstChars key)
{
    return cxHashGet(this->kvs, cxHashStrKey(key));
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

cxBool cxRect4fContainsRect4f(cxRect4f r1,cxRect4f r2)
{
    cxFloat x1 = r1.x;
    cxFloat y1 = r1.y;
    cxFloat x2 = r1.x + r1.w;
    cxFloat y2 = r1.y + r1.h;
    cxFloat x3 = r2.x;
    cxFloat y3 = r2.y;
    cxFloat x4 = r2.x + r2.w;
    cxFloat y4 = r2.y + r2.h;
    bool c1 = (x1 >=x3 && x1 < x4) || (x3 >= x1 && x3 <= x2);
    bool c2 = (y1 >=y3 && y1 < y4) || (y3 >= y1 && y3 <= y2);
    return (c1 && c2) ? true : false;
}

cxBoxTex2f cxRect4fToBoxTex2f(cxRect4f box,cxSize2f texsize)
{
    cxBoxTex2f rv = cxBoxTex2fDefault();
    cxFloat x = box.x / texsize.w;
    cxFloat y = box.y / texsize.h;
    cxFloat w = box.w / texsize.w;
    cxFloat h = box.h / texsize.h;
    rv.lb = cxTex2fv(x, y + h);
    rv.rb = cxTex2fv(x + w, y + h);
    rv.lt = cxTex2fv(x, y);
    rv.rt = cxTex2fv(x + w, y);
    return rv;
}

cxColor4f cxColor4fValue(cxColor4fRange cv)
{
    cxColor4f color;
    color.r = kmClamp(cv.v.r + cv.r.r * CX_RAND_11f(), 0.0f, 1.0f);
    color.g = kmClamp(cv.v.g + cv.r.g * CX_RAND_11f(), 0.0f, 1.0f);
    color.b = kmClamp(cv.v.b + cv.r.b * CX_RAND_11f(), 0.0f, 1.0f);
    color.a = kmClamp(cv.v.a + cv.r.a * CX_RAND_11f(), 0.0f, 1.0f);
    return color;
}

cxVec2f cxVec2fValue(cxVec2fRange rv)
{
    cxVec2f pos;
    pos.x = rv.v.x + rv.r.x * CX_RAND_11f();
    pos.y = rv.v.y + rv.r.y * CX_RAND_11f();
    return pos;
}

cxBool cxBox2fContainPoint(const cxBox4f box,const cxVec2f pos)
{
    return (pos.x >= box.l && pos.x <= box.r && pos.y >= box.b && pos.y <= box.t);
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