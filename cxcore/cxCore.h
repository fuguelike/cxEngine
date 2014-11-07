//
//  cxFramework.h
//  cxFramework
//
//  Created by xuhua on 4/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxCore_h
#define cxCore_cxCore_h

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

#define CX_TARGET_PLATFORM         CX_PLATFORM_UNKNOWN

// mac
#if defined(CX_TARGET_MAC)
#undef  CX_TARGET_PLATFORM
#define CX_TARGET_PLATFORM         CX_PLATFORM_MAC
#endif

// iphone
#if defined(CX_TARGET_IOS)
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

#define CX_EXTERN   extern

#ifndef CX_MIN
#define CX_MIN(v1,v2) (((v1) > (v2)) ? (v2) : (v1))
#endif

#ifndef CX_MAX
#define CX_MAX(v1,v2) (((v1) < (v2)) ? (v2) : (v1))
#endif

#ifndef CX_SWAP
#define CX_SWAP(v1, v2) {typeof(v1) _temp_=(v1);v1=v2;v2=_temp_;}
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
#include <errno.h>
#include <float.h>
#include <time.h>
#include <sys/types.h>
#include <stddef.h>

typedef int             cxOff;
typedef char            cxChar;
typedef int             cxInt;
typedef long            cxLong;
typedef int8_t          cxInt8;
typedef int16_t         cxInt16;
typedef int32_t         cxInt32;
typedef int64_t         cxInt64;

typedef const void *    cxConstAny;
typedef void *          cxAny;
typedef size_t          cxSize;

typedef unsigned char   cxByte;
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
typedef cxChar *        cxChars;
typedef cxUChar *       cxUChars;

#define CX_ENGINE_VERSION               201

#define CX_ATTR_UNUSED                  __attribute__ ((__unused__))

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define CX_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400
    #define CX_DEPRECATED_ATTRIBUTE __declspec(deprecated)
#else
    #define CX_DEPRECATED_ATTRIBUTE
#endif

//max logger length
#define CX_MAX_LOGGER_LENGTH            1024

//max type signature name length
#define MAX_TYPE_SIGNATURE_SIZE         1024

#define CX_INLINE                       static inline

//#define NDEBUG 1

#if !defined(NDEBUG)

#define CX_LOGGER(format,...)           cxUtilInfo(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_ERROR(format,...)            cxUtilError(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_WARN(format,...)             cxUtilWarn(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_ASSERT(cond,format,...)                              \
do{                                                             \
    if(!(cond))                                                 \
    cxUtilAssert(__FILE__,__LINE__,format, ##__VA_ARGS__);      \
    assert(cond);                                               \
}while(0)

#define CX_ASSERT_FALSE(format,...)     CX_ASSERT(false,format,##__VA_ARGS__)

#define CX_ASSERT_TYPE(_o_,_t_)         CX_ASSERT(CX_INSTANCE_OF(_o_,_t_),"object type (%s) error,should is "#_t_,CX_NAME_OF(_o_))

#else

#define CX_LOGGER(format,...)

#define CX_ASSERT(cond,...)

#define CX_ASSERT_TYPE(_o_,_t_)

#define CX_ASSERT_FALSE(format,...)

#define CX_ERROR(format,...)            cxUtilError(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_WARN(format,...)             cxUtilWarn(__FILE__,__LINE__,format, ##__VA_ARGS__)

#endif

#define CX_ASSERT_VALUE(_o_,_t_,_n_)    _t_ _n_ = (_t_)(_o_);CX_ASSERT_TYPE(_n_,_t_)

#define CX_ASSERT_THIS(_o_,_t_)         CX_ASSERT_VALUE(_o_,_t_,this)

#define CX_RETURN(cond,...)             if(cond)return __VA_ARGS__

//type define

#define CX_TYPE_REG(_t_)                __##_t_##RegisterFunc()

#define CX_BEG(_t_,_b_)                                         \
CX_ATTR_UNUSED static cxConstType _t_##TypeName = #_t_;         \
typedef struct _t_ *_t_;                                        \
void __##_t_##AutoInit(_t_ this);                               \
void __##_t_##AutoFree(_t_ this);                               \
void __##_t_##AutoType(cxAny this);                             \
struct _t_ {

#define CX_END(_t_,_b_) };                                      \
CX_ATTR_UNUSED static cxAny __##_t_##CreateFunc()               \
{                                                               \
    return CX_CREATE(_t_);                                      \
}                                                               \
CX_ATTR_UNUSED static cxAny __##_t_##AllocFunc()                \
{                                                               \
    return CX_ALLOC(_t_);                                       \
}                                                               \
CX_ATTR_UNUSED static void __##_t_##RegisterFunc()              \
{                                                               \
    __cxTypeRegisterType(_t_##TypeName,_b_##TypeName,           \
    __##_t_##CreateFunc,                                        \
    __##_t_##AllocFunc,                                         \
    __##_t_##AutoType);                                         \
}

#define CX_DEF(_t_,_b_)             CX_BEG(_t_,_b_) struct _b_ _b_;

//type imp cxAny = cxType
#define CX_TYPE(_t_,_b_)            void __##_t_##AutoType(cxAny this){

#define CX_INIT(_t_,_b_)            };void __##_t_##AutoInit(_t_ this){__##_b_##AutoInit((_b_)this);

#define CX_FREE(_t_,_b_)            };void __##_t_##AutoFree(_t_ this){

#define CX_TERM(_t_,_b_)            __##_b_##AutoFree((_b_)this);}

//object mem manage

#define CX_ALLOC(_t_)               __cxObjectAlloc(_t_##TypeName,sizeof(struct _t_),(cxAny)__##_t_##AutoInit,(cxAny)__##_t_##AutoFree)

#define CX_CREATE(_t_)              __cxObjectCreate(_t_##TypeName,sizeof(struct _t_),(cxAny)__##_t_##AutoInit,(cxAny)__##_t_##AutoFree)

#define CX_RETAIN(_o_)              __cxObjectRetain(_o_)

#define CX_RELEASE(_o_)             __cxObjectRelease(_o_)

#define CX_AUTO(_o_)                __cxObjectAutoRelease(_o_)

#define CX_RETAIN_SWAP(_s_,_d_)     do{CX_RELEASE(_s_);(_s_)=(cxAny)(_d_);CX_RETAIN(_s_);}while(0)

#define CX_RETAIN_SET(_f_,_v_)      do{_f_ = (_v_);CX_RETAIN(_f_);}while(0)

#define CX_INSTANCE_OF(_o_,_t_)     cxObjectInstanceOf(_o_,_t_##TypeName)

#define CX_NAME_OF(_o_)             ((cxObject)_o_)->cxType

#define CX_TYPE_OF(_t_,_o_)         ((_t_)(_o_))

//method

#define CX_METHOD_DEF(_r_,_n_,...)  _r_ (*_n_)(__VA_ARGS__)

#define CX_METHOD_GET(_d_,_m_,...)  (((_m_) != NULL)?((_m_)(__VA_ARGS__)):(_d_))

#define CX_METHOD_RUN(_m_,...)      if((_m_) != NULL)(_m_)(__VA_ARGS__)

#define CX_METHOD_SET(_m_,_f_)      _m_ = _f_

//method overwrite short micro
#define CX_SET(_t_,_o_,_n_,_f_)                                 \
CX_ASSERT_TYPE(_o_, _t_);                                       \
CX_METHOD_SET(CX_TYPE_OF(_t_, _o_)->_n_, _f_)

//field

#define CX_FIELD_DEF(_var_)         _var_

#define CX_FIELD_GET(_t_,_vt_, _n_)                             \
CX_INLINE _vt_ _t_##Get##_n_(cxAny pthis)                       \
{                                                               \
    CX_ASSERT_THIS(pthis,_t_);                                  \
    return this->_n_;                                           \
}

#define CX_FIELD_SET(_t_,_vt_, _n_)                             \
CX_INLINE void _t_##Set##_n_(cxAny pthis,const _vt_ value)      \
{                                                               \
    CX_ASSERT_THIS(pthis,_t_);                                  \
    this->_n_ = value;                                          \
}

#define CX_FIELD_IMP(_t_,_vt_, _n_)                             \
CX_FIELD_GET(_t_,_vt_, _n_)                                     \
CX_FIELD_SET(_t_,_vt_, _n_)

//signal and slot

typedef struct cxSignal cxSignal;
typedef struct cxSlot cxSlot;
struct cxSignal {cxSignal *prev,*next;cxSlot *slot;cxAny func;cxAny object;};
struct cxSlot{cxSignal **signal;cxSignal *slot;};

#define CX_SIGNAL_ALLOC(_name_) cxSignal *_name_

#define CX_SIGNAL_RELEASE(_signal_)                             \
do{                                                             \
    cxSignal *_tmp_ = NULL;                                     \
    cxSignal *_ele_ = NULL;                                     \
    DL_FOREACH_SAFE(_signal_, _ele_, _tmp_){                    \
        DL_DELETE(_signal_, _ele_);                             \
        _ele_->slot->slot = NULL;                               \
        _ele_->slot->signal = NULL;                             \
        allocator->free(_ele_);                                 \
    }                                                           \
}while(0)

//only use at CX_SIGNAL_FIRE micro
#define CX_SIGNAL_TYPE(...) (void (*)(cxAny,##__VA_ARGS__))

#define CX_SIGNAL_FIRE(_signal_,_ft_,...)                       \
do{                                                             \
    cxSignal *_ele_ = NULL;                                     \
    DL_FOREACH(_signal_, _ele_){                                \
        (_ft_(_ele_->func))(_ele_->object,##__VA_ARGS__);       \
    }                                                           \
}while(0)

#define CX_SLOT_ALLOC(name)      cxSlot name

#define CX_SLOT_RELEASE(_slot_)                                 \
if(_slot_.slot != NULL && _slot_.signal != NULL){               \
    DL_DELETE(*_slot_.signal,_slot_.slot);                      \
    allocator->free(_slot_.slot);                               \
    _slot_.signal = NULL;                                       \
    _slot_.slot = NULL;                                         \
}

#define CX_SLOT_CONNECT(_signal_,_object_,_slot_,_func_)        \
do{                                                             \
    CX_SLOT_RELEASE(_object_->_slot_);                          \
    cxSignal *_new_ = allocator->malloc(sizeof(cxSignal));      \
    _new_->func = _func_;                                       \
    _new_->object = _object_;                                   \
    _new_->slot = &_object_->_slot_;                            \
    _object_->_slot_.slot = _new_;                              \
    _object_->_slot_.signal = &_signal_;                        \
    DL_APPEND(_signal_,_new_);                                  \
}while(0)

#define CX_CON(_t_,_src_,_n_,_dst_,_f_)                         \
CX_ASSERT_TYPE(_src_, _t_);                                     \
CX_SLOT_CONNECT(CX_TYPE_OF(_t_, _src_)->_n_, _dst_, _n_, _f_)

//event

typedef struct cxEvent cxEvent;
typedef void (*cxEventFunc)(cxAny sender);
struct cxEvent{cxEvent *prev,*next;cxEventFunc func;};

#define CX_EVENT_ALLOC(_n_) cxEvent *_n_

#define CX_EVENT_APPEND(_event_,_func_)                         \
if(_func_ != NULL){                                             \
    cxEvent *_newptr_ = allocator->malloc(sizeof(cxEvent));     \
    _newptr_->func = _func_;                                    \
    DL_APPEND(_event_, _newptr_);                               \
}

#define CX_EVENT_PREPEND(_event_,_func_)                        \
if(_func_ != NULL){                                             \
    cxEvent *_newptr_ = allocator->malloc(sizeof(cxEvent));     \
    _newptr_->func = _func_;                                    \
    DL_PREPEND(_event_, _newptr_);                              \
}

#define CX_EVENT_DEL(_event_,_func_)                            \
if(_event_ != NULL){                                            \
    cxEvent *_tmp_ = NULL;                                      \
    cxEvent *_ele_ = NULL;                                      \
    DL_FOREACH_SAFE(_event_, _ele_, _tmp_){                     \
        CX_CONTINUE(_ele_->func != _func_);                     \
        DL_DELETE(_event_, _ele_);                              \
        allocator->free(_ele_);                                 \
    }                                                           \
}

#define CX_EVENT_RELEASE(_event_)                               \
if(_event_ != NULL){                                            \
    cxEvent *_tmp_ = NULL;                                      \
    cxEvent *_ele_ = NULL;                                      \
    DL_FOREACH_SAFE(_event_, _ele_, _tmp_){                     \
        DL_DELETE(_event_, _ele_);                              \
        allocator->free(_ele_);                                 \
    }                                                           \
}

#define CX_EVENT_FIRE(_sender_,_event_)                         \
if(_sender_ != NULL){                                           \
    cxEvent *_ele_ = NULL;                                      \
    cxEvent *_tmp_=NULL;                                        \
    DL_FOREACH_SAFE(_sender_->_event_, _ele_,_tmp_){            \
        _ele_->func(_sender_);                                  \
    }                                                           \
}

//append event short micro
#define CX_ADD(_t_,_o_,_n_,_f_)                                 \
CX_ASSERT_TYPE(_o_, _t_);                                       \
CX_EVENT_APPEND(CX_TYPE_OF(_t_, _o_)->_n_, _f_)

//
cxUInt32 cxAtomicAddInt32(cxInt32 *p, cxInt32 x);

cxUInt32 cxAtomicSubInt32(cxInt32 *p, cxInt32 x);

typedef cxAny (*cxMallocFunc)(cxSize size);

typedef cxAny (*cxReallocFunc)(cxAny ptr,cxSize size);

typedef cxAny (*cxCallocFunc)(cxInt num,cxSize size);

typedef void (*cxFreeFunc)(cxAny ptr);

typedef cxChars(*cxStrdupFunc)(cxConstChars s);

typedef struct {
    cxMallocFunc    malloc;
    cxReallocFunc   realloc;
    cxCallocFunc    calloc;
    cxFreeFunc      free;
    cxStrdupFunc    strdup;
}cxAllocator;

extern const cxAllocator *allocator;

void cxAllocatorInit();

void cxAllocatorFree();

typedef void (*cxObjectFunc)(cxAny this);

typedef cxAny (*cxAnyFunc)(cxAny object);

cxAny __cxObjectAlloc(cxConstType type,cxInt size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

cxAny __cxObjectCreate(cxConstType type, cxInt size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

void  __cxObjectRetain(cxAny ptr);

void  __cxObjectRelease(cxAny ptr);

cxAny __cxObjectAutoRelease(cxAny ptr);

void  __cxTypeRegisterType(cxConstType tt,cxConstType bb,cxAny (*create)(),cxAny (*alloc)(),void (*autoType)(cxAny));

//must completed cxUtilPrint function with platform
void cxUtilPrint(cxConstChars type,cxConstChars file,cxInt line,cxConstChars format,va_list ap);

void cxUtilInfo(cxConstChars file, cxInt line, cxConstChars format, ...);

void cxUtilError(cxConstChars file, cxInt line, cxConstChars format, ...);

void cxUtilWarn(cxConstChars file, cxInt line, cxConstChars format, ...);

void cxUtilAssert(cxConstChars file, cxInt line, cxConstChars format, ...);

cxDouble cxTimestamp();

//base type define
CX_BEG(cxObject,cxObject)
    cxConstType cxType;
    cxInt cxRefcount;
    cxObjectFunc cxFree;
CX_END(cxObject,cxObject)

cxAny cxObjectType(cxAny object);

cxAny cxObjectProperty(cxAny object,cxConstChars key);

cxBool cxObjectInstanceOf(cxAny object,cxConstType type);

void cxCorePush(cxAny object);

void cxCorePop();

cxAny cxCoreTop();

void cxCoreInit();

void cxCoreFree();

#endif
