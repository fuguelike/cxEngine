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

#define CX_EXTERN       extern
#define CX_MIN(x,y)     (((x) > (y)) ? (y) : (x))
#define CX_MAX(x,y)     (((x) < (y)) ? (y) : (x))
#define CX_SWAP(v1,v2)  {__typeof__(v1) _temp_=(v1);v1=v2;v2=_temp_;}
#define CX_SWAP32(i)    ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define CX_SWAP16(i)    ((i & 0x00ff) << 8 | (i & 0xff00) >> 8)

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
#include <kazmath/kazmath.h>

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
#define CX_ATTRIBUTE_UNUSED     __attribute__ ((__unused__))
#else
#define CX_ATTRIBUTE_UNUSED
#endif

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define CX_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400
#define CX_DEPRECATED_ATTRIBUTE __declspec(deprecated)
#else
#define CX_DEPRECATED_ATTRIBUTE
#endif

#define CX_INVALID_INDEX            -1

#define CX_MAX_LOGGER_LENGTH        1024

#if !defined(NDEBUG)

#define CX_LOGGER(format,...)       cxUtilInfo(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_ERROR(format,...)        cxUtilError(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_WARN(format,...)         cxUtilWarn(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_ASSERT(cond,format,...)                              \
do{                                                             \
    if(!(cond))                                                 \
    cxUtilAssert(__FILE__,__LINE__,format, ##__VA_ARGS__);      \
    assert(cond);                                               \
}while(0)

#define CX_ASSERT_FALSE(format,...) CX_ASSERT(false,format,##__VA_ARGS__)

#else

#define CX_LOGGER(format,...)

#define CX_ASSERT(cond,...)

#define CX_ASSERT_FALSE(format,...)

#define CX_ERROR(format,...)         cxUtilError(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_WARN(format,...)          cxUtilWarn(__FILE__,__LINE__,format, ##__VA_ARGS__)

#endif


#endif















