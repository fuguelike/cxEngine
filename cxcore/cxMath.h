//
//  cxBase.h
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxMath_h
#define cxEngine_cxMath_h

#include <kazmath/kazmath.h>
#include <kazmath/GL/matrix.h>
#include "cxCore.h"
#include "cxAnyArray.h"

CX_C_BEGIN

#define cxFloatEqu(v1,v2) kmAlmostEqual(v1,v2)

// -1.0f <-> 1.0f
#define CX_RAND_11f() ((2.0f*((cxFloat)(rand()%RAND_MAX)/(cxFloat)RAND_MAX))-1.0f)

// 0.0f <-> 1.0f
#define CX_RAND_01f() ((cxFloat)(rand()%RAND_MAX)/(cxFloat)RAND_MAX)

typedef struct {
    cxFloat v;  //base value
    cxFloat r;  //random value
}cxFloatRange;
#define cxFloatRandValue(fv) ((fv).v + (fv).r * CX_RAND_11f())
#define cxFloatRangeValue(v,r) (cxFloatRange){v,r}

typedef struct {
    cxFloat min;
    cxFloat max;
}cxRange2f;
#define cxRange2fv(min,max) (cxRange2f){min,max}
#define cxRange2fEqu(p1,p2) (cxFloatEqu((p1).min ,(p2).min) && cxFloatEqu((p1).max,(p2).max))

typedef struct {
    cxInt min;
    cxInt max;
}cxRange2i;
#define cxRange2iv(min,max) (cxRange2i){min,max}
#define cxRange2ix(v)       (cxRange2i){v,v}
#define cxRange2iEqu(p1,p2) ((p1).min == (p2).min && (p1).max == (p2).max)

typedef struct {
    cxInt x;
    cxInt y;
}cxVec2i;
#define cxVec2iv(x,y)   (cxVec2i){x,y}
#define cxVec2iEqu(p1,p2)   ((p1).x == (p2).x && (p1).y == (p2).y)

typedef struct {
    cxInt x;
    cxInt y;
    cxInt z;
}cxVec3i;
#define cxVec3iv(x,y,z)   (cxVec3i){x,y,z}
#define cxVec3iEqu(p1,p2)   ((p1).x == (p2).x && (p1).y == (p2).y && (p1).z == (p2).z)

typedef kmMat4 cxMatrix4f;

typedef kmVec2 cxVec2f;
#define cxVec2fx(x)             (cxVec2f){x,x}
#define cxVec2fv(x,y)           (cxVec2f){x,y}
#define cxVec2fIsError(v)       (isinf(v.x) && isinf(v.y))
#define cxVec2fEqu(p1,p2)       (cxFloatEqu((p1).x,(p2).x) && cxFloatEqu((p1).y,(p2).y))
#define cxVec2fAngle(a)         atan2f((a).y, (a).x)
#define cxVec2fLength(a)        sqrtf((a).x*(a).x + (a).y*(a).y)
#define cxVec2fZero(v)          (cxFloatEqu((v).x, 0.0f) && cxFloatEqu((v).y, 0.0f))
#define cxVec2fOne(v)           (cxFloatEqu((v).x, 1.0f) && cxFloatEqu((v).y, 1.0f))
#define cxVec2fTo2i(v)          cxVec2iv((v).x,(v).y)

typedef kmVec3 cxVec3f;
#define cxVec3fv(x,y,z)         (cxVec3f){x,y,z}
#define cxVec3fEqu(p1,p2)       (cxFloatEqu((p1).x,(p2).x) && cxFloatEqu((p1).y,(p2).y) && cxFloatEqu((p1).z,(p2).z))
#define cxVec3fZero(v)          (cxFloatEqu((v).x, 0.0f) && cxFloatEqu((v).y, 0.0f) && cxFloatEqu((v).z, 0.0f))

static const cxVec3f cxVec3fX  = cxVec3fv(1.0f,0.0f,0.0f);
static const cxVec3f cxVec3fY  = cxVec3fv(0.0f,1.0f,0.0f);
static const cxVec3f cxVec3fZ  = cxVec3fv(0.0f,0.0f,1.0f);

typedef struct {
    cxVec2f b;      //begin point
    cxVec2f e;      //end point
}cxLineVec2f;
#define cxLine4fv(x1,y1,x2,y2)  (cxLineVec2f){{x1,y1},{x2,y2}}
#define cxLineVec2fv(b,e)       (cxLineVec2f){{(b).x,(b).y},{(e).x,(e).y}}

typedef struct {
    cxFloat v1;
    cxFloat v2;
    cxFloat v3;
    cxFloat v4;
}cxAssist4f;
#define cxAssist4fv(v1,v2,v3,v4) (cxAssist4f){v1,v2,v3,v4}

typedef struct {
    cxUInt8 r;
    cxUInt8 g;
    cxUInt8 b;
    cxUInt8 a;
}cxColor4b;
#define cxColor4bv(r,g,b,a)     (cxColor4b){r,g,b,a}

typedef struct {
    cxFloat r;
    cxFloat g;
    cxFloat b;
    cxFloat a;
}cxColor4f;
#define cxColor4fv(r,g,b,a)      (cxColor4f){r,g,b,a}
#define cxColor4fx(v)            (cxColor4f){v,v,v,v}
#define cxColor4fEqu(c1,c2)      (cxFloatEqu((c1).r,(c2).r)&&cxFloatEqu((c1).g,(c2).g)&&cxFloatEqu((c1).b,(c2).b)&&cxFloatEqu((c1).a,(c2).a))

typedef struct {
    cxUInt8 r;
    cxUInt8 g;
    cxUInt8 b;
}cxColor3b;
#define cxColor3bv(r,g,b)       (cxColor3b){r,g,b}
#define cxColor3bx(v)           (cxColor3b){v,v,v}

typedef struct {
    cxFloat r;
    cxFloat g;
    cxFloat b;
}cxColor3f;
#define cxColor3fToColor4f(c)   cxColor4fv((c).r,(c).g,(c).b,1.0f)
#define cxColor3fv(r,g,b)       (cxColor3f){r,g,b}
#define cxColor3fx(v)           (cxColor3f){v,v,v}
#define cxColor3fEqu(c1,c2)     (cxFloatEqu((c1).r,(c2).r) && cxFloatEqu((c1).g,(c2).g) && cxFloatEqu((c1).b,(c2).b))

//colors define

static const cxColor3f cxWHITE   = cxColor3fv(1.00f, 1.00f, 1.00f);
static const cxColor3f cxYELLOW  = cxColor3fv(1.00f, 1.00f, 0.00f);
static const cxColor3f cxBLUE    = cxColor3fv(0.00f, 0.00f, 1.00f);
static const cxColor3f cxGREEN   = cxColor3fv(0.00f, 1.00f, 0.00f);
static const cxColor3f cxRED     = cxColor3fv(1.00f, 0.00f, 0.00f);
static const cxColor3f cxMAGENTA = cxColor3fv(1.00f, 0.00f, 1.00f);
static const cxColor3f cxBLACK   = cxColor3fv(0.00f, 0.00f, 0.00f);
static const cxColor3f cxORANGE  = cxColor3fv(1.00f, 0.50f, 0.00f);
static const cxColor3f cxGRAY    = cxColor3fv(0.65f, 0.65f, 0.65f);
static const cxColor3f cxPURPLE  = cxColor3fv(0.63f, 0.13f, 0.94f);

typedef struct {
    cxInt w;
    cxInt h;
}cxSize2i;
#define cxSize2iv(w,h)          (cxSize2i){w,h}
#define cxSize2iEqu(s1,s2)      ((s1).w == (s2).w && (s1.h) == (s2.h))

typedef struct {
    cxFloat w;
    cxFloat h;
}cxSize2f;
#define cxSize2fv(w,h)          (cxSize2f){w,h}
#define cxSize2fEqu(s1,s2)      (cxFloatEqu((s1).w,(s2).w) &&  cxFloatEqu((s1).h,(s2).h))
#define cxSize2fZero(v)         (cxFloatEqu((v).w, 0) && cxFloatEqu((v).h, 0))

typedef struct {
    cxFloat u;
    cxFloat v;
}cxTex2f;
#define cxTex2fv(u,v)       (cxTex2f){u,v}
#define cxTex2fEqu(v1,v2)   (cxFloatEqu((v1).u,(v2).u) && cxFloatEqu((v1).v,(v2).v))

typedef struct {
    cxFloat l;
    cxFloat r;
    cxFloat t;
    cxFloat b;
}cxBox4f;
#define cxBox4fv(l,r,t,b)       (cxBox4f){l,r,t,b}
#define cxBox4fx(v)             (cxBox4f){v,v,v,v}
#define cxBox4fEqu(v1,v2)       (cxFloatEqu((v1).l,(v2).l) && cxFloatEqu((v1).r,(v2).r) && cxFloatEqu((v1).t,(v2).t) && cxFloatEqu((v1).b,(v2).b))
#define cxBoxLeftLine(box)      cxLineVec2fv(cxVec2fv(box.l, box.t), cxVec2fv(box.l, box.b))
#define cxBoxRightLine(box)     cxLineVec2fv(cxVec2fv(box.r, box.b), cxVec2fv(box.r, box.t))
#define cxBoxTopLine(box)       cxLineVec2fv(cxVec2fv(box.l, box.t), cxVec2fv(box.r, box.t))
#define cxBoxBottomLine(box)    cxLineVec2fv(cxVec2fv(box.l, box.b), cxVec2fv(box.r, box.b))

typedef struct {
    cxInt l;
    cxInt r;
    cxInt t;
    cxInt b;
}cxBox4i;
#define cxBox4iv(l,r,t,b)       (cxBox4i){l,r,t,b}

typedef struct {
    cxFloat x;
    cxFloat y;
    cxFloat w;
    cxFloat h;
}cxRect4f;
#define cxRect4fv(x,y,w,h)  (cxRect4f){x,y,w,h}

typedef struct {
    cxInt x;
    cxInt y;
    cxInt w;
    cxInt h;
}cxRect4i;
#define cxRect4iv(x,y,w,h)  (cxRect4i){x,y,w,h}

typedef struct {
    cxVec2f lt;
    cxVec2f rt;
    cxVec2f rb;
    cxVec2f lb;
}cxBoxVec2f;
#define cxBoxVec2fFromBox4f(box) (cxBoxVec2f){{(box).l,(box).t},{(box).r,(box).t},{(box).r,(box).b},{(box).l,(box).b}}

typedef struct {
    cxVec3f lt;
    cxVec3f lb;
    cxVec3f rt;
    cxVec3f rb;
}cxBoxVec3f;

typedef struct {
    cxVec3f vertices;
    cxColor4f colors;
    cxTex2f texcoords;
}cxPoint;

typedef struct {
    cxPoint lt;
    cxPoint lb;
    cxPoint rt;
    cxPoint rb;
}cxBoxPoint;

typedef struct {
    cxPoint b;
    cxPoint e;
}cxLinePoint;

typedef struct {
    cxTex2f lt;
    cxTex2f lb;
    cxTex2f rt;
    cxTex2f rb;
}cxBoxTex2f;
#define cxBoxTex2Scale(v)   (cxBoxTex2f){cxTex2fv(0.0f, 0.0f),cxTex2fv(0.0f, v),cxTex2fv(v, 0.0f),cxTex2fv(v, v)}
#define cxBoxTex2Equ(v1,v2) (cxTex2fEqu((v1).lt,(v2).lt) && cxTex2fEqu((v1).lb,(v2).lb) && cxTex2fEqu((v1).rt,(v2).rt) && cxTex2fEqu((v1).rb,(v2).rb))

static const cxBoxTex2f cxBoxTex2fDefault = {
    cxTex2fv(0.0f, 0.0f),
    cxTex2fv(0.0f, 1.0f),
    cxTex2fv(1.0f, 0.0f),
    cxTex2fv(1.0f, 1.0f)
};

cxBoxTex2f cxBoxTex2fFlip(cxBoxTex2f tex,cxBool flipx,cxBool flipy,cxBox4f pixel);

cxBoxTex2f cxRect4fToBoxTex2f(cxRect4f box,cxSize2f texsize);

typedef struct {
    cxColor4f lt;
    cxColor4f lb;
    cxColor4f rt;
    cxColor4f rb;
}cxBoxColor4f;

static const cxBoxColor4f cxBoxColor4fDefault = {
    cxColor4fv(1,1,1,1),
    cxColor4fv(1,1,1,1),
    cxColor4fv(1,1,1,1),
    cxColor4fv(1,1,1,1)
};

typedef enum {
    cxTextAlignNone     = 0,
    cxTextAlignLeft     = 1,
    cxTextAlignCenter   = 2,
    cxTextAlignRight    = 3,
}cxTextAlign;

typedef struct {
    //font size
    cxFloat size;
    cxTextAlign align;
    cxColor4f color;
    cxFloat strokeWidth;
    cxColor4f strokeColor;
    cxVec2f strokeOffset;
    cxBool boldFont;
}cxTextAttr;

typedef struct {
    cxColor4f v;
    cxColor4f r;
}cxColor4fRange;
cxColor4f cxColor4fRandValue(cxColor4fRange r);
#define cxColor4fRangeValue(r1,g1,b1,a1,r2,g2,b2,a2)  (cxColor4fRange){{r1,g1,b1,a1},{r2,g2,b2,a2}}

typedef struct {
    cxVec2f v;
    cxVec2f r;
}cxVec2fRange;
cxVec2f cxVec2fRandValue(cxVec2fRange rv);
#define cxVec2fRangeValue(vx,vy,rx,ry)    (cxVec2fRange){{vx,vy},{rx,ry}}

typedef struct {
    cxVec3f v;
    cxVec3f r;
}cxVec3fRange;
cxVec3f cxVec3fRandValue(cxVec3fRange rv);
#define cxVec3fRangeValue(vx,vy,vz,rx,ry,rz)    (cxVec3fRange){{vx,vy,vz},{rx,ry,rz}}

CX_INLINE cxInt cxCharToInt(cxUChar c)
{
    cxInt v = tolower(c);
    return v >= 'a' ? (v - 'a' + 10) : (v - '0');
}

CX_INLINE cxBox4f cxBox4fNewForExtents(const cxVec2f c, const cxFloat hw, const cxFloat hh)
{
    return cxBox4fv(c.x - hw, c.x + hw, c.y + hh, c.y - hh);
}
//r radius
CX_INLINE cxBox4f cxBox4fNewForCircle(const cxVec2f p, const cxFloat r)
{
    return cxBox4fNewForExtents(p, r, r);
}
//from center scale size
CX_INLINE cxBox4f cxBox4fScale(cxBox4f box,cxFloat scale)
{
    return cxBox4fv(box.l * scale, box.r * scale, box.t * scale, box.b * scale);
}
//from center scale size
CX_INLINE cxBox4f cxBox4fScaleSize(cxBox4f box,cxFloat scale)
{
    cxFloat w = box.r - box.l;
    cxFloat h = box.t - box.b;
    cxFloat x = box.l + w/2.0f;
    cxFloat y = box.t - h/2.0f;
    return cxBox4fNewForExtents(cxVec2fv(x, y), (w/2.0f) * scale, (h/2.0f) * scale);
}
//
CX_INLINE cxFloat cxRound(cxFloat v)
{
    if(v > 0){
        return v + kmEpsilon;
    }else if(v < 0){
        return v - kmEpsilon;
    }else{
        return v;
    }
}
//
CX_INLINE cxVec2f cxVec2fRound(cxVec2f v)
{
    return cxVec2fv(cxRound(v.x), cxRound(v.y));
}
//
CX_INLINE cxVec2f cxVec2fSub(cxVec2f pV1, cxVec2f pV2)
{
    return cxVec2fv(pV1.x - pV2.x, pV1.y - pV2.y);
}

CX_INLINE cxVec2f cxVec2fAdd(cxVec2f pV1, cxVec2f pV2)
{
    return cxVec2fv(pV1.x + pV2.x, pV1.y + pV2.y);
}

CX_INLINE cxVec2f cxVec2fScale1f(cxVec2f pIn, cxFloat s)
{
    return cxVec2fv(pIn.x * s, pIn.y * s);
}

CX_INLINE cxVec2f cxVec2fScale2f(cxVec2f pIn, cxVec2f s)
{
    return cxVec2fv(pIn.x * s.x, pIn.y * s.y);
}

CX_INLINE cxVec2f cxVec2fMidPoint(cxVec2f v1, cxVec2f v2)
{
    cxVec2f sum = cxVec2fAdd(v1, v2);
    return cxVec2fv(sum.x / 2.0f, sum.y / 2.0f);
}

cxVec2i cxViewPosToIndex(cxSize2f msize,cxVec2f pos,cxSize2f esize);

cxVec2f cxViewIndexToPos(cxSize2f msize,cxVec2i idx,cxSize2f esize);

cxFloat cxVec2fRadiansBetween(cxVec2f v1, cxVec2f v2);

cxFloat cxVec2fDistanceBetween(cxVec2f v1, cxVec2f v2);

cxBool cxPolygonContainPoint(cxAnyArray polygon,cxVec2f tp);

cxVec2f cxCardinalSplineAt(cxVec2f p0, cxVec2f p1, cxVec2f p2, cxVec2f p3, cxFloat tension, cxFloat t);

cxColor4f cxCharsToColor4f(cxConstChars color,cxColor4f dv);

cxFloat cxBezier2(cxFloat a, cxFloat b, cxFloat c, cxFloat t);

cxFloat cxBezier3(cxFloat a, cxFloat b, cxFloat c, cxFloat d, cxFloat t);

cxBool cxBox4fIntersects(const cxBox4f a, const cxBox4f b);

cxBool cxBox4fContainsBox4f(const cxBox4f bb, const cxBox4f other);

cxBool cxBox4fContainPoint(cxBox4f box,cxVec2f pos);

//if r1 contains r2 return true
cxBool cxRect4fContainsRect4f(cxRect4f r1,cxRect4f r2);

typedef enum {
    cxBoxEdgeNone   = 0,
    cxBoxEdgeLeft   = 1 << 0,
    cxBoxEdgeRight  = 1 << 1,
    cxBoxEdgeTop    = 1 << 2,
    cxBoxEdgeBottom = 1 << 3,
    cxBoxEdgeInside = 1 << 4,
    cxBoxEdgeFour   = cxBoxEdgeLeft|cxBoxEdgeRight|cxBoxEdgeTop|cxBoxEdgeBottom,
}cxBoxEdge;

typedef struct {
    cxVec2f point;
    cxBoxEdge edge;
}cxIntersect;

typedef struct {
    cxIntersect ps[4];
    cxInt num;
    cxBoxEdge edges;
}cxLineBoxInfo;
//
cxInt cxVec2fAtLineLR(cxLineVec2f l, cxVec2f pt);
//
cxVec2f cxTwoLineIntersectPoint(cxLineVec2f l1,cxLineVec2f l2);
//
cxBool cxTwoLineIsIntersect(cxLineVec2f l1,cxLineVec2f l2,cxVec2f *ip);
//
cxLineBoxInfo cxLineBoxIsIntersect(cxLineVec2f l, cxBox4f box,cxBoxEdge edges,cxFloat scale);

CX_C_END

#endif
