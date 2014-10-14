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
#define CX_RAND_11f() ((2.0f*((cxFloat)rand()/(cxFloat)RAND_MAX))-1.0f)

// 0.0f <-> 1.0f
#define CX_RAND_01f() ((cxFloat)rand()/(cxFloat)RAND_MAX)

typedef struct {
    cxFloat v;  //base value
    cxFloat r;  //random value
}cxFloatRange;
#define cxFloatValue(fv) ((fv).v + (fv).r * CX_RAND_11f())
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
#define cxVec2fEqu(p1,p2)       (cxFloatEqu((p1).x,(p2).x) && cxFloatEqu((p1).y,(p2).y))
#define cxVec2fAngle(a)         atan2f((a).y, (a).x)
#define cxVec2fLength(a)        sqrtf((a).x*(a).x + (a).y*(a).y)
#define cxVec2fZero(v)          (cxFloatEqu((v).x, 0.0f) && cxFloatEqu((v).y, 0.0f))
#define cxVec2fOne(v)           (cxFloatEqu((v).x, 1.0f) && cxFloatEqu((v).y, 1.0f))
#define cxVec2fTo2i(v)          cxVec2iv((v).x,(v).y)

typedef kmVec3 cxVec3f;
#define cxVec3fv(x,y,z)         (cxVec3f){x,y,z}
#define cxVec3fEqu(p1,p2)       (cxFloatEqu((p1).x,(p2).x) && cxFloatEqu((p1).y,(p2).y) && cxFloatEqu((p1).z,(p2).z))

static const cxVec3f cxVec3fX  = cxVec3fv(1.0f,0.0f,0.0f);
static const cxVec3f cxVec3fY  = cxVec3fv(0.0f,1.0f,0.0f);
static const cxVec3f cxVec3fZ  = cxVec3fv(0.0f,0.0f,1.0f);

typedef struct {
    cxFloat v1;
    cxFloat v2;
    cxFloat v3;
    cxFloat v4;
}cxAssist4f;

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
#define cxColor4fEqu(c1,c2)      (cxFloatEqu((c1).r,(c2).r)&&cxFloatEqu((c1).g,(c2).g)&&cxFloatEqu((c1).b,(c2).b)&&cxFloatEqu((c1).a,(c2).a))

typedef struct {
    cxFloat r;
    cxFloat g;
    cxFloat b;
}cxColor3f;
#define cxColor3fToColor4f(c)   cxColor4fv((c).r,(c).g,(c).b,1.0f)
#define cxColor3fv(r,g,b)       (cxColor3f){r,g,b}
#define cxColor3fEqu(c1,c2)     (cxFloatEqu((c1).r,(c2).r) && cxFloatEqu((c1).g,(c2).g) && cxFloatEqu((c1).b,(c2).b))
#define cxColor3bv(r,g,b)       (cxColor3f){(cxFloat)(r)/255.0f,(cxFloat)(g)/255.0f,(cxFloat)(b)/255.0f}

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
#define cxTex2fv(u,v)   (cxTex2f){u,v}

typedef struct {
    cxFloat l;
    cxFloat r;
    cxFloat t;
    cxFloat b;
}cxBox4f;
#define cxBox4fv(l,r,t,b) (cxBox4f){l,r,t,b}
#define cxBox4fInit() cxBox4fv(INT32_MAX,INT32_MIN,INT32_MIN,INT32_MAX)

typedef struct {
    cxInt l;
    cxInt r;
    cxInt t;
    cxInt b;
}cxBox4i;

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
    cxPoint p1;
    cxPoint p2;
}cxLinePoint;

typedef struct {
    cxTex2f lt;
    cxTex2f lb;
    cxTex2f rt;
    cxTex2f rb;
}cxBoxTex2f;
#define cxBoxTex2fDefault() (cxBoxTex2f){cxTex2fv(0.0f, 0.0f),cxTex2fv(0.0f, 1.0f),cxTex2fv(1.0f, 0.0f),cxTex2fv(1.0f, 1.0f)}
#define cxBoxTex2Scale(v)   (cxBoxTex2f){cxTex2fv(0.0f, 0.0f),cxTex2fv(0.0f, v),cxTex2fv(v, 0.0f),cxTex2fv(v, v)}

cxBoxTex2f cxBoxTex2fFlip(cxBoxTex2f tex,cxBool flipx,cxBool flipy);

cxRect4f cxBoxTex2fToRect4f(cxBoxTex2f box);

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
    cxTextAlignCenter       = 0x33, ///< Horizontal center and vertical center.
    cxTextAlignTop          = 0x13, ///< Horizontal center and vertical top.
    cxTextAlignTopRight     = 0x12, ///< Horizontal right and vertical top.
    cxTextAlignRight        = 0x32, ///< Horizontal right and vertical center.
    cxTextAlignBottomRight  = 0x22, ///< Horizontal right and vertical bottom.
    cxTextAlignBottom       = 0x23, ///< Horizontal center and vertical bottom.
    cxTextAlignBottomLeft   = 0x21, ///< Horizontal left and vertical bottom.
    cxTextAlignLeft         = 0x31, ///< Horizontal left and vertical center.
    cxTextAlignTopLeft      = 0x11, ///< Horizontal left and vertical top.
}cxTextAlign;

typedef struct {
    cxFloat size;
    cxTextAlign align;
    cxSize2f viewSize;
}cxTextAttr;

typedef struct {
    cxColor4f v;
    cxColor4f r;
}cxColor4fRange;
cxColor4f cxColor4fValue(cxColor4fRange r);
#define cxColor4fRangeValue(r1,g1,b1,a1,r2,g2,b2,a2)  (cxColor4fRange){{r1,g1,b1,a1},{r2,g2,b2,a2}}

typedef struct {
    cxVec2f v;
    cxVec2f r;
}cxVec2fRange;
cxVec2f cxVec2fValue(cxVec2fRange rv);
#define cxVec2fRangeValue(vx,vy,rx,ry)    (cxVec2fRange){{vx,vy},{rx,ry}}

cxFloat cxVec2fRadiansBetween(cxVec2f v1, cxVec2f v2);

cxBool cxPointsContainPoint(cxAnyArray polygon,cxVec2f tp);

cxVec2f cxCardinalSplineAt(cxVec2f p0, cxVec2f p1, cxVec2f p2, cxVec2f p3, cxFloat tension, cxFloat t);

CX_INLINE  cxVec2f cxVec2fSub(cxVec2f pV1, cxVec2f pV2)
{
    return cxVec2fv(pV1.x - pV2.x, pV1.y - pV2.y);
}

CX_INLINE  cxVec2f cxVec2fAdd(cxVec2f pV1, cxVec2f pV2)
{
    return cxVec2fv(pV1.x + pV2.x, pV1.y + pV2.y);
}

CX_INLINE cxVec2f cxVec2fScale(cxVec2f pIn, cxFloat s)
{
    return cxVec2fv(pIn.x * s, pIn.y * s);
}

CX_INLINE cxVec2f cxVec2fMidPoint(cxVec2f v1, cxVec2f v2)
{
    cxVec2f sum = cxVec2fAdd(v1, v2);
    return cxVec2fv(sum.x / 2.0f, sum.y / 2.0f);
}

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

CX_INLINE cxVec2f cxRoundVec2f(cxVec2f v)
{
    return cxVec2fv(cxRound(v.x), cxRound(v.y));
}

CX_INLINE cxFloat cxBezier2(cxFloat a, cxFloat b, cxFloat c, cxFloat t)
{
    return (powf(1.0f-t,2.0f)*a+2.0f*t*(1.0f-t)*b+powf(t,2.0f)*c);
}

CX_INLINE cxFloat cxBezier3(cxFloat a, cxFloat b, cxFloat c, cxFloat d, cxFloat t)
{
    return (powf(1.0f-t,3.0f)*a+3.0f*t*(powf(1.0f-t,2.0f))*b+3.0f*powf(t,2.0f)*(1.0f-t)*c+powf(t,3.0f)*d);
}

CX_INLINE cxBool cxBox4fIntersects(const cxBox4f a, const cxBox4f b)
{
    return (a.l <= b.r && b.l <= a.r && a.b <= b.t && b.b <= a.t);
}

CX_INLINE cxBool cxBox4fContainsBox4f(const cxBox4f bb, const cxBox4f other)
{
    return (bb.l <= other.l && bb.r >= other.r && bb.b <= other.b && bb.t >= other.t);
}

CX_INLINE cxBool cxBox4fContainPoint(cxBox4f box,cxVec2f pos)
{
    return (pos.x >= box.l && pos.x <= box.r && pos.y >= box.b && pos.y <= box.t);
}

//if r1 contains r2 return true
CX_INLINE cxBool cxRect4fContainsRect4f(cxRect4f r1,cxRect4f r2)
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

CX_C_END

#endif
