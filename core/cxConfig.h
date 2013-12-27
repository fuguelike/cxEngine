//
//  cxConfig.h
//  cxEngine
//  类名规则:   cxTest
//  宏规则:    CX_MICRO
//  方法规则:  cxMethodRun
//  字段:     cxField
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxConfig_h
#define cxEngine_cxConfig_h

#define CX_PLATFORM_UNKNOWN            0
#define CX_PLATFORM_IOS                1
#define CX_PLATFORM_ANDROID            2
#define CX_PLATFORM_WIN32              3
#define CX_PLATFORM_MARMALADE          4
#define CX_PLATFORM_LINUX              5
#define CX_PLATFORM_BADA               6
#define CX_PLATFORM_BLACKBERRY         7
#define CX_PLATFORM_MAC                8
#define CX_PLATFORM_NACL               9

#define CX_TARGET_PLATFORM             CX_PLATFORM_UNKNOWN

// mac
#if defined(CX_TARGET_OS_MAC)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM         CX_PLATFORM_MAC
#endif

// iphone
#if defined(CX_TARGET_OS_IPHONE)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM         CX_PLATFORM_IOS
#endif

// android
#if defined(ANDROID)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM         CX_PLATFORM_ANDROID
#endif

// win32
#if defined(WIN32) && defined(_WINDOWS)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM         CX_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM         CX_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM         CX_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM         CX_PLATFORM_BADA
#endif

// qnx
#if defined(__QNCX__)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM     CX_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM     CX_PLATFORM_NACL
#endif

#define CX_UNUSED_PARAM(p) (void)p

#ifdef __cplusplus
#define CX_C_BEGIN       extern "C" {
#define CX_C_END         }
#else
#define CX_C_BEGIN
#define CX_C_END
#endif

#ifndef CX_MIN
#define CX_MIN(x,y) (((x) > (y)) ? (y) : (x))
#endif

#ifndef CX_MAX
#define CX_MAX(x,y) (((x) < (y)) ? (y) : (x))
#endif

#ifndef CX_SWAP
#define CX_SWAP(v1, v2, type) {type _temp_=(v1);v1=v2;v2=_temp_;}
#endif

#ifndef CX_SWAP32
#define CX_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#endif

#ifndef CX_SWAP16
#define CX_SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)
#endif

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <libxml/xmlreader.h>
#include <kazmath/kazmath.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#define CX_ENGINE_VERSION   100

typedef int             cxOff;
typedef char            cxChar;
typedef int             cxInt;
typedef int             cxIndex;
typedef long            cxLong;
typedef int8_t          cxInt8;
typedef int16_t         cxInt16;
typedef int32_t         cxInt32;
typedef int64_t         cxInt64;

typedef void *          cxPointer;
typedef cxPointer       cxAny;
typedef size_t          cxSize;

typedef unsigned char   cxUChar;
typedef unsigned int    cxUInt;
typedef unsigned long   cxULong;
typedef uint8_t         cxUInt8;
typedef uint16_t        cxUInt16;
typedef uint32_t        cxUInt32;
typedef uint64_t        cxUInt64;
typedef bool            cxBool;

typedef float           cxFloat;
typedef double          cxDouble;

typedef const char *    cxConstType;
typedef const char *    cxConstChars;

#ifdef __GNUC__
#define CX_UNUSED_ATTRIBUTE  __attribute__ ((__unused__))
#else
#define CX_UNUSED_ATTRIBUTE
#endif

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define CX_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
#define CX_DEPRECATED_ATTRIBUTE __declspec(deprecated)
#else
#define CX_DEPRECATED_ATTRIBUTE
#endif

#define CX_INVALID_INDEX   -1

#define CX_MAX_LOGGER_LENGTH         1024

#if !defined(NDEBUG)

#define CX_LOGGER(format, ...)       cxUtilInfo(__FILE__,__LINE__,format, ##__VA_ARGS__)
#define CX_ERROR(format, ...)        cxUtilError(__FILE__,__LINE__,format, ##__VA_ARGS__)
#define CX_WARN(format, ...)         cxUtilWarn(__FILE__,__LINE__,format, ##__VA_ARGS__)
#define CX_ASSERT(cond,format,...)  \
do{\
    if(!(cond)) \
    cxUtilAssert(__FILE__,__LINE__,format, ##__VA_ARGS__);  \
    assert(cond);   \
}while(0)

#else

#define CX_LOGGER(format, ...)
#define CX_ASSERT(cond,...)
#define CX_ERROR(format, ...)         cxUtilError(__FILE__,__LINE__,format, ##__VA_ARGS__)
#define CX_WARN(format, ...)          cxUtilWarn(__FILE__,__LINE__,format, ##__VA_ARGS__)

#endif

#define cxFloatEqu kmAlmostEqual

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
#define cxVec2fv(x,y)           (cxVec2f){x,y}
#define cxVec2fEqu(p1,p2)       (cxFloatEqu((p1).x,(p2).x) && cxFloatEqu((p1).y,(p2).y))
#define cxVec2fAngle(a)         atan2f((a).y, (a).x)
#define cxVec2fMagnitude(a)     sqrtf((a).x*(a).x + (a).y*(a).y)

typedef kmVec3 cxVec3f;
#define cxVec3fv(x,y,z)         (cxVec3f){x,y,z}
#define cxVec3fEqu(p1,p2)       (cxFloatEqu((p1).x,(p2).x) && cxFloatEqu((p1).y,(p2).y) && cxFloatEqu((p1).z,(p2).z))

typedef struct {
    kmScalar v1;
    kmScalar v2;
    kmScalar v3;
    kmScalar v4;
} cxAssist4f;

typedef struct {
    kmScalar r;
    kmScalar g;
    kmScalar b;
    kmScalar a;
} cxColor4f;
#define cxColor4fv(r,g,b,a)     (cxColor4f){r,g,b,a}
#define cxColor4fEqu(c1,c2)     (cxFloatEqu((c1).r,(c2).r)&&cxFloatEqu((c1).g,(c2).g)&&cxFloatEqu((c1).b,(c2).b)&&cxFloatEqu((c1).a,(c2).a))
#define cxColor4bv(r,g,b,a)     (cxColor4f){(cxFloat)(r)/255.0f,(cxFloat)(g)/255.0f,(cxFloat)(b)/255.0f,(cxFloat)(a)/255.0f}

typedef struct {
    kmScalar r;
    kmScalar g;
    kmScalar b;
} cxColor3f;
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
} cxSize2i;
#define cxSize2iv(w,h)          (cxSize2i){w,h}

typedef struct {
    kmScalar w;
    kmScalar h;
} cxSize2f;
#define cxSize2fv(w,h)          (cxSize2f){w,h}
#define cxSize2fEqu(s1,s2)      (cxFloatEqu((s1).w,(s2).w) &&  cxFloatEqu((s1).h,(s2).h))
#define cxSize2Zero(v)          (kmAlmostEqual((v).w, 0) && kmAlmostEqual((v).h, 0))

typedef struct {
    kmScalar u;
    kmScalar v;
} cxTexCoord2f;
#define cxTex2fv(u,v)   (cxTexCoord2f){u,v}

typedef struct {
    kmScalar l;
    kmScalar r;
    kmScalar t;
    kmScalar b;
} cxBox4f;
#define cxBox4fv(l,r,t,b) (cxBox4f){l,r,t,b}
#define cxBox4fInit() cxBox4fv(INT32_MAX,INT32_MIN,INT32_MIN,INT32_MAX)

typedef struct {
    cxInt l;
    cxInt r;
    cxInt t;
    cxInt b;
} cxBox4i;

typedef struct {
    kmScalar x;
    kmScalar y;
    kmScalar w;
    kmScalar h;
} cxRect4f;
#define cxRect4fv(x,y,w,h)  (cxRect4f){x,y,w,h}

typedef struct {
    cxInt x;
    cxInt y;
    cxInt w;
    cxInt h;
} cxRect4i;
#define cxRect4iv(x,y,w,h)  (cxRect4i){x,y,w,h}

typedef struct {
    cxVec2f lt;
    cxVec2f rt;
    cxVec2f rb;
    cxVec2f lb;
} cxBoxVec2f;
#define cxBoxVec2fFromBox4f(box) (cxBoxVec2f){{(box).l,(box).t},{(box).r,(box).t},{(box).r,(box).b},{(box).l,(box).b}}

typedef struct {
    cxVec2f *vs;
    cxInt num;
} cxPolygon;

typedef struct {
    cxVec3f lt;
    cxVec3f lb;
    cxVec3f rt;
    cxVec3f rb;
} cxBoxVec3f;

typedef struct {
    cxVec3f vertices;
    cxColor4f colors;
    cxTexCoord2f texcoords;
} cxPoint;

typedef struct {
    cxPoint lt;
    cxPoint lb;
    cxPoint rt;
    cxPoint rb;
} cxBoxPoint;

typedef struct {
    cxPoint p1;
    cxPoint p2;
} cxLinePoint;

typedef struct {
    cxTexCoord2f lt;
    cxTexCoord2f lb;
    cxTexCoord2f rt;
    cxTexCoord2f rb;
} cxBoxTex2f;
#define cxBoxTex2fDefault() (cxBoxTex2f){cxTex2fv(0.0f, 0.0f),cxTex2fv(0.0f, 1.0f),cxTex2fv(1.0f, 0.0f),cxTex2fv(1.0f, 1.0f)}
#define cxBoxTex2Scale(v)   (cxBoxTex2f){cxTex2fv(0.0f, 0.0f),cxTex2fv(0.0f, v),cxTex2fv(v, 0.0f),cxTex2fv(v, v)}

cxRect4f cxBoxTex2fToRect4f(cxBoxTex2f box);

cxBoxTex2f cxRect4fToBoxTex2f(cxRect4f box,cxSize2f texsize);

typedef struct {
    cxColor4f lt;
    cxColor4f lb;
    cxColor4f rt;
    cxColor4f rb;
} cxBoxColor4f;

typedef struct {
    cxFloat size;
    cxBool bold;
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

cxVec2f cxCardinalSplineAt(cxVec2f p0, cxVec2f p1, cxVec2f p2, cxVec2f p3, cxFloat tension, cxFloat t);

cxFloat cxBezier2(cxFloat a, cxFloat b, cxFloat c, cxFloat t);

cxFloat cxBezier3(cxFloat a, cxFloat b, cxFloat c, cxFloat d, cxFloat t);

cxBool cxBox2fContainPoint(const cxBox4f box,const cxVec2f pos);

cxBool cxPolygonContainPoint(const cxPolygon *polygon,const cxVec2f tp);

//if r1 contains r2 return true
cxBool cxRect4fContainsRect4f(cxRect4f r1,cxRect4f r2);
#endif















