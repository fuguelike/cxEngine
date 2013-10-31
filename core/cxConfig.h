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
#define CX_UNUSED __attribute__ ((__unused__))
#else
#define CX_UNUSED
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

#define CX_OBJECT_BEG(_t_)                              \
static CX_UNUSED cxConstType _t_##AutoType = #_t_;      \
typedef struct _t_ * _t_;                               \
void _t_##AutoInit(_t_ this);                           \
void _t_##AutoFree(_t_ this);                           \
struct _t_ {


#define CX_OBJECT_END(_t_)                              };

//method

#define CX_METHOD_DEF(_t_,_n_)          _t_ _n_

#define CX_METHOD_SET(_m_,_f_)          _m_=_f_

#define CX_METHOD_RUN(_m_,...)          if(_m_ != NULL){_m_(__VA_ARGS__);}

#define CX_METHOD_GET(_v_,_m_,...)      ((_m_ != NULL)?(_m_(__VA_ARGS__)):(_v_))

typedef void (*cxObjectFunc)(cxPointer this);

typedef cxBool (*cxXMLReadAttrFunc)(cxAny pxml,cxAny pnew, xmlTextReaderPtr reader);

//base type define
CX_OBJECT_BEG(cxObject)
    cxConstType cxType;
    cxUInt cxRefcount;
    cxObjectFunc cxFree;
    cxInt cxTag;
    cxAny cxRoot;
    cxInt cxBase;
    cxInt cxSize;
    CX_METHOD_DEF(cxXMLReadAttrFunc,XMLReadAttr);
CX_OBJECT_END(cxObject)

cxBool cxObjectIsType(cxAny pobj,cxConstType type);

cxBool cxObjectXMLReadAttr(cxAny pobj,cxAny newobj, xmlTextReaderPtr reader);

void cxObjectSetXMLReadFunc(cxAny obj,cxXMLReadAttrFunc func);

cxBool cxObjectXMLReadRun(cxAny obj,cxAny pobj,xmlTextReaderPtr reader);

cxAny cxObjectRoot(cxAny obj);

void cxObjectSetRoot(cxAny obj,cxAny root);

void cxObjectSetTag(cxAny obj,cxInt tag);

cxInt cxObjectGetTag(cxAny obj);

#define CX_RETURN(cond,...)       if(cond)return __VA_ARGS__

#define CX_BREAK(cond)            if(cond)break

#define CX_CONST_STRING(n,...)    cxChar n[128]={0};snprintf(n, 128, ##__VA_ARGS__)

#define CX_OBJECT_DEF(_t_,_b_)    CX_OBJECT_BEG(_t_) struct _b_ super;

#define CX_OBJECT_INIT(_t_,_b_)   void _t_##AutoInit(_t_ this){_b_##AutoInit((_b_)this);do{

#define CX_OBJECT_FREE(_t_,_b_)   }while(0);};void _t_##AutoFree(_t_ this){do{

#define CX_OBJECT_TERM(_t_,_b_)   }while(0);_b_##AutoFree((_b_)this);}

#define CX_ALLOC(_t_)             cxObjectAlloc(_t_##AutoType,sizeof(struct _t_),(cxObjectFunc)_t_##AutoInit,(cxObjectFunc)_t_##AutoFree)

#define CX_CREATE(_t_)            cxObjectCreate(_t_##AutoType,sizeof(struct _t_),(cxObjectFunc)_t_##AutoInit,(cxObjectFunc)_t_##AutoFree)

#define CX_RETAIN(_o_)            cxObjectRetain(_o_)

#define CX_RELEASE(_o_)           cxObjectRelease(_o_)

#define CX_AUTOFREE(_o_)          cxObjectAutoRelease(_o_)

#define CX_RETAIN_SWAP(_s_,_d_)   do{CX_RELEASE(_s_);(_s_)=(cxAny)(_d_);CX_RETAIN(_s_);}while(0)

#endif















