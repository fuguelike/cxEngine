//
//  cxBase.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMath.h"

//FF FF FF FF
cxColor4f cxCharsToColor4f(cxConstChars color,cxColor4f dv)
{
    if(color == NULL){
        return dv;
    }
    cxInt len = (cxInt)strlen(color);
    cxFloat v = 0;
    if(len >= 2){
        //set r
        v = (cxCharToInt(color[0]) << 4) | cxCharToInt(color[1]);
        dv.r = v/255.0f;
    }
    if(len >= 4){
        //set g
        v = (cxCharToInt(color[2]) << 4) | cxCharToInt(color[3]);
        dv.g = v/255.0f;
    }
    if(len >= 6){
        //set b
        v = (cxCharToInt(color[4]) << 4) | cxCharToInt(color[5]);
        dv.b = v/255.0f;
    }
    if(len >= 8){
        v = (cxCharToInt(color[6]) << 4) | cxCharToInt(color[7]);
        dv.a = v/255.0f;
    }
    return dv;
}

cxFloat cxBezier2(cxFloat a, cxFloat b, cxFloat c, cxFloat t)
{
    return powf(1.0f-t,2.0f)*a+2.0f*t*(1.0f-t)*b+powf(t,2.0f)*c;
}

cxFloat cxBezier3(cxFloat a, cxFloat b, cxFloat c, cxFloat d, cxFloat t)
{
    return powf(1.0f-t,3.0f)*a+3.0f*t*(powf(1.0f-t,2.0f))*b+3.0f*powf(t,2.0f)*(1.0f-t)*c+powf(t,3.0f)*d;
}

cxBool cxBox4fIntersects(const cxBox4f a, const cxBox4f b)
{
    return (a.l <= b.r && b.l <= a.r && a.b <= b.t && b.b <= a.t);
}

cxBool cxBox4fContainsBox4f(const cxBox4f bb, const cxBox4f other)
{
    return (bb.l <= other.l && bb.r >= other.r && bb.b <= other.b && bb.t >= other.t);
}

cxBool cxBox4fContainPoint(cxBox4f box,cxVec2f pos)
{
    return (pos.x >= box.l && pos.x <= box.r && pos.y >= box.b && pos.y <= box.t);
}

//if r1 contains r2 return true
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
    cxBool c1 = (x1 >=x3 && x1 < x4) || (x3 >= x1 && x3 <= x2);
    cxBool c2 = (y1 >=y3 && y1 < y4) || (y3 >= y1 && y3 <= y2);
    return (c1 && c2) ? true : false;
}


cxFloat cxVec2fRadiansBetween(cxVec2f v1, cxVec2f v2)
{
    if(cxVec2fEqu(v1, v2)){
        return FLT_EPSILON;
    }
    cxVec2f delta = cxVec2fSub(v1, v2);
    return cxVec2fAngle(delta);
}

cxFloat cxVec2fDistanceBetween(cxVec2f v1, cxVec2f v2)
{
    return kmVec2DistanceBetween(&v1, &v2);
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
    cxVec2f rv = cxVec2fv(0, 0);
    rv.x = (p0.x*b1 + p1.x*b2 + p2.x*b3 + p3.x*b4);
    rv.y = (p0.y*b1 + p1.y*b2 + p2.y*b3 + p3.y*b4);
	return rv;
}

cxBoxTex2f cxBoxTex2fFlip(cxBoxTex2f tex,cxBool flipx,cxBool flipy,cxBox4f pixel)
{
    tex.lb.u += pixel.l;
    tex.rb.u += pixel.r;
    tex.lt.u += pixel.l;
    tex.rt.u += pixel.r;
    tex.lb.v += pixel.b;
    tex.rb.v += pixel.b;
    tex.lt.v += pixel.t;
    tex.rt.v += pixel.t;
    cxBoxTex2f ret = tex;
    if(flipx){
        ret.lb.u = tex.rb.u;
        ret.rb.u = tex.lb.u;
        ret.lt.u = tex.rt.u;
        ret.rt.u = tex.lt.u;
    }
    if(flipy){
        ret.lb.v = tex.lt.v;
        ret.rb.v = tex.rt.v;
        ret.lt.v = tex.lb.v;
        ret.rt.v = tex.rb.v;
    }
    return ret;
}

cxBoxTex2f cxRect4fToBoxTex2f(cxRect4f box,cxSize2f texsize)
{
    cxBoxTex2f rv = cxBoxTex2fDefault;
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

cxVec2i cxViewPosToIndex(cxSize2f msize,cxVec2f pos,cxSize2f esize)
{
    cxInt x = (pos.x + msize.w/2.0f) / esize.w;
    cxInt y = (pos.y + msize.h/2.0f) / esize.h;
    return cxVec2iv(x, y);
}

cxVec2f cxViewIndexToPos(cxSize2f msize,cxVec2i idx,cxSize2f esize)
{
    cxFloat x = idx.x * esize.w + esize.w/2.0f - msize.w/2.0f;
    cxFloat y = idx.y * esize.h + esize.h/2.0f - msize.h/2.0f;;
    return cxVec2fv(x, y);
}

cxColor4f cxColor4fRandValue(cxColor4fRange cv)
{
    cxColor4f color;
    color.r = kmClamp(cv.v.r + cv.r.r * CX_RAND_11f(), 0.0f, 1.0f);
    color.g = kmClamp(cv.v.g + cv.r.g * CX_RAND_11f(), 0.0f, 1.0f);
    color.b = kmClamp(cv.v.b + cv.r.b * CX_RAND_11f(), 0.0f, 1.0f);
    color.a = kmClamp(cv.v.a + cv.r.a * CX_RAND_11f(), 0.0f, 1.0f);
    return color;
}

cxVec2f cxVec2fRandValue(cxVec2fRange rv)
{
    cxVec2f pos;
    pos.x = rv.v.x + rv.r.x * CX_RAND_11f();
    pos.y = rv.v.y + rv.r.y * CX_RAND_11f();
    return pos;
}

cxVec3f cxVec3fRandValue(cxVec3fRange rv)
{
    cxVec3f pos;
    pos.x = rv.v.x + rv.r.x * CX_RAND_11f();
    pos.y = rv.v.y + rv.r.y * CX_RAND_11f();
    pos.z = rv.v.z + rv.r.z * CX_RAND_11f();
    return pos;
}

cxBool cxPolygonContainPoint(cxAnyArray polygon,cxVec2f tp)
{
    cxBool c = false;
    cxInt num = cxAnyArrayLength(polygon);
    const cxVec2f *vs = cxAnyArrayPtr(polygon, cxVec2f);
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

// 判断点在有向直线的左侧还是右侧.
// 返回值:1: 点在射线左侧; 0: 点在射线上; -1: 点在射线右侧
cxInt cxVec2fAtLineLR(cxLineVec2f l, cxVec2f pt)
{
    l.b.x -= pt.x;
    l.b.y -= pt.y;
    l.e.x -= pt.x;
    l.e.y -= pt.y;
    cxInt nRet = l.b.x * l.e.y - l.b.y * l.e.x;
    if (nRet == 0)
        return 0;
    else if (nRet > 0)
        return 1;
    else if (nRet < 0)
        return -1;
    return 0;
    
}
// k = (y2-y1)/(x2-x1)
// y = k*x + (y1-x1*k);
//获取直线y=kx+b的k和b(两点式转为点斜式)
CX_INLINE cxFloat cxLineGetKB(cxLineVec2f l, cxFloat *b)
{
    cxFloat k = (l.e.y-l.b.y)/(l.e.x-l.b.x);
    *b = l.b.y - l.b.x * k;
    return k;
}
//两直线的交点，如果已经相交
cxVec2f cxTwoLineIntersectPoint(cxLineVec2f l1,cxLineVec2f l2)
{
    cxFloat k1,b1,k2,b2,x,y;
    k1 = cxLineGetKB(l1, &b1);
    k2 = cxLineGetKB(l2, &b2);
    //not intersect
    if((isinf(k1) && isinf(k2)) || cxFloatEqu(k1, k2)){
        return cxVec2fv(INFINITY, INFINITY);
    }
    if(isinf(k1)){
        x = l1.b.x;
        y = k2 * x + b2;
    }else if(isinf(k2)){
        x = l2.b.x;
        y = k1 * x + b1;
    }else{
        x = (b2-b1)/(k1-k2);
        y = k1 * x + b1;
    }
    return cxVec2fv(x, y);
}
// 判断两条线段是否相交，并返回交点
cxBool cxTwoLineIsIntersect(cxLineVec2f l1,cxLineVec2f l2,cxVec2f *ip)
{
    cxInt n1 = cxVec2fAtLineLR(l2, l1.b);
    cxInt n2 = cxVec2fAtLineLR(l2, l1.e);
    if (n1 * n2 > 0){
        return false;
    }
    cxInt n3 = cxVec2fAtLineLR(l1, l2.b);
    cxInt n4 = cxVec2fAtLineLR(l1, l2.e);
    if (n3 * n4 > 0){
        return false;
    }
    //计算交点
    *ip = cxTwoLineIntersectPoint(l1,l2);
    return true;
}

//判断线段是否与box相交,并获取的交点信息
cxLineBoxInfo cxLineBoxIsIntersect(cxLineVec2f l, cxBox4f box,cxBoxEdge edges,cxFloat scale)
{
    cxFloat dscale = 1.0f/scale;
    cxBool isscale = !cxFloatEqu(scale, 0);
    if(isscale){
        l.b = cxVec2fScale1f(l.b, scale);
        l.e = cxVec2fScale1f(l.e, scale);
        box = cxBox4fScale(box, scale);
    }
    cxLineBoxInfo info = {0};
    cxVec2f ip;
    cxIntersect *it;
    if((edges & cxBoxEdgeInside) && cxBox4fContainPoint(box, l.b)){
        it = &info.ps[info.num++];
        it->edge = cxBoxEdgeInside;
        it->point = isscale?cxVec2fScale1f(l.b,dscale):l.b;
    }
    if((edges & cxBoxEdgeInside) && cxBox4fContainPoint(box, l.e)){
        it = &info.ps[info.num++];
        it->edge = cxBoxEdgeInside;
        it->point = isscale?cxVec2fScale1f(l.e,dscale):l.e;
    }
    //left line
    if((edges & cxBoxEdgeLeft) && cxTwoLineIsIntersect(l, cxBoxLeftLine(box), &ip)){
        CX_ASSERT(!cxVec2fIsError(ip), "intersece error");
        it = &info.ps[info.num++];
        it->edge = cxBoxEdgeLeft;
        it->point = isscale?cxVec2fScale1f(ip,dscale):ip;
    }
    //bottom line
    if((edges & cxBoxEdgeBottom) && cxTwoLineIsIntersect(l, cxBoxBottomLine(box), &ip)){
        CX_ASSERT(!cxVec2fIsError(ip), "intersece error");
        it = &info.ps[info.num++];
        it->edge = cxBoxEdgeBottom;
        it->point = isscale?cxVec2fScale1f(ip,dscale):ip;
    }
    //right line
    if((edges & cxBoxEdgeRight) && cxTwoLineIsIntersect(l, cxBoxRightLine(box), &ip)){
        CX_ASSERT(!cxVec2fIsError(ip), "intersece error");
        it = &info.ps[info.num++];
        it->edge = cxBoxEdgeRight;
        it->point = isscale?cxVec2fScale1f(ip,dscale):ip;
    }
    //top line
    if((edges & cxBoxEdgeTop) && cxTwoLineIsIntersect(l, cxBoxTopLine(box), &ip)){
        CX_ASSERT(!cxVec2fIsError(ip), "intersece error");
        it = &info.ps[info.num++];
        it->edge = cxBoxEdgeTop;
        it->point = isscale?cxVec2fScale1f(ip,dscale):ip;
    }
    return info;
}



