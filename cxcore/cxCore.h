//
//  cxFramework.h
//  cxFramework
//
//  Created by xuhua on 4/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxCore_h
#define cxCore_cxCore_h

//#define NDEBUG 1

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
#define CX_SWAP16(i)  ((i & 0x00ff) << 8 | (i & 0xff00) >> 8)
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
#include <ctype.h>
#include "uthash.h"

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

#define CX_ERROR(format,...)            cxUtilError(__FILE__,__LINE__,format, ##__VA_ARGS__)

#define CX_WARN(format,...)             cxUtilWarn(__FILE__,__LINE__,format, ##__VA_ARGS__)

#if !defined(NDEBUG)
#define CX_LOGGER(format,...)           cxUtilInfo(__FILE__,__LINE__,format, ##__VA_ARGS__)
#define CX_ASSERT(cond,format,...)                              \
do{                                                             \
    bool _ret_= (cond);                                         \
    if(!_ret_)                                                  \
    cxUtilAssert(__FILE__,__LINE__,format, ##__VA_ARGS__);      \
    assert(_ret_);                                              \
}while(0)
#define CX_ASSERT_FALSE(format,...)     CX_ASSERT(false,format,##__VA_ARGS__)
#define CX_ASSERT_TYPE(_o_,_t_)         CX_ASSERT(CX_INSTANCE_OF(_o_,_t_),"object type (%s) error,should is "#_t_,CX_NAME_OF(_o_))
#define CX_ASSERT_POINTER(_o_,_p_)      CX_ASSERT(CX_VALID_PTR(_o_,_p_),"pointer not valid");
#else
#define CX_LOGGER(format,...)
#define CX_ASSERT(cond,...)
#define CX_ASSERT_TYPE(_o_,_t_)
#define CX_ASSERT_FALSE(format,...)
#define CX_ASSERT_POINTER(_o_,_p_)
#endif

#define CX_ASSERT_VALUE(_o_,_t_,_n_)    _t_ _n_ = (_t_)(_o_);CX_ASSERT_TYPE(_n_,_t_)

#define CX_ASSERT_THIS(_o_,_t_)         CX_ASSERT_VALUE(_o_,_t_,this)

#define CX_RETURN(cond,...)             if(cond)return __VA_ARGS__

//type define

#define CX_SET_TYPE(_t_)                __##_t_##TypeSetFunc()

#define CX_BEG(_t_,_b_)                                         \
CX_ATTR_UNUSED static cxConstType _t_##TypeName = #_t_;         \
CX_ATTR_UNUSED static cxType _t_##TypePointer = NULL;           \
typedef struct _t_ *_t_;                                        \
void __##_t_##AutoInit(_t_ this);                               \
void __##_t_##AutoFree(_t_ this);                               \
void __##_t_##AutoType(cxType this);                            \
struct _t_ {

#define CX_END(_t_,_b_) };                                      \
CX_ATTR_UNUSED static cxAny __##_t_##AllocFunc()                \
{                                                               \
    cxObject this = allocator->malloc(sizeof(struct _t_));      \
    CX_ASSERT(this != NULL, "memory alloc error");              \
    this->cxRefcount = 1;                                       \
    this->cxType = cxTypesGetType(_t_##TypeName);               \
    this->cxAutoFree = (void(*)(cxAny))__##_t_##AutoFree;       \
    __##_t_##AutoInit((_t_)this);                               \
    CX_CALL(this, cxInit, CX_M(void));                          \
    return this;                                                \
}                                                               \
CX_ATTR_UNUSED static cxAny __##_t_##CreateFunc()               \
{                                                               \
    return CX_AUTO_RELEASE(__##_t_##AllocFunc());               \
}                                                               \
CX_ATTR_UNUSED static void __##_t_##TypeSetFunc()               \
{                                                               \
    _t_##TypePointer = cxTypeNew(_t_##TypeName,_b_##TypeName,   \
    __##_t_##CreateFunc,__##_t_##AllocFunc,                     \
    __##_t_##AutoType,sizeof(struct _t_));                      \
}

#define CX_DEF(_t_,_b_)             CX_BEG(_t_,_b_) struct _b_ _b_;

//type imp cxAny = cxType

#define CX_TYPE(_t_,_b_)            void __##_t_##AutoType(cxType this){

#define CX_INIT(_t_,_b_)            };void __##_t_##AutoInit(_t_ this){__##_b_##AutoInit((_b_)this);

#define CX_FREE(_t_,_b_)            };void __##_t_##AutoFree(_t_ this){

#define CX_TERM(_t_,_b_)            __##_b_##AutoFree((_b_)this);}

//object mem manage

#define CX_ALLOC(_t_)               __##_t_##AllocFunc()

#define CX_CREATE(_t_)              __##_t_##CreateFunc()

#define CX_RETAIN(_o_)              __cxObjectRetain(_o_)

#define CX_RELEASE(_o_)             __cxObjectRelease(_o_)

#define CX_AUTO_RELEASE(_o_)        __cxObjectAutoRelease(_o_)

#define CX_RETAIN_SWAP(_s_,_d_)     do{CX_RELEASE(_s_);(_s_)=(cxAny)(_d_);CX_RETAIN(_s_);}while(0)

#define CX_RETAIN_SET(_f_,_v_)      do{_f_ = (_v_);CX_RETAIN(_f_);}while(0)

//type

#define CX_INSTANCE_OF(_o_,_t_)     cxInstanceOf(_o_,_t_##TypeName)

#define CX_VALID_PTR(_o_,_p_)       cxValidPointer(_o_,_p_)

#define CX_NAME_OF(_o_)             ((cxType)((cxObject)_o_)->cxType)->typeName

#define CX_TYPE_OF(_t_,_o_)         ((_t_)(_o_))

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

#define CX_FIELD_REF(_t_,_vt_, _n_)                             \
CX_INLINE void _t_##Set##_n_(cxAny pthis,const _vt_ value)      \
{                                                               \
    CX_ASSERT_THIS(pthis,_t_);                                  \
    CX_RETAIN_SWAP(this->_n_,value);                            \
}

#define CX_FIELD_IMO(_t_,_vt_, _n_)                             \
CX_FIELD_GET(_t_,_vt_, _n_)                                     \
CX_FIELD_REF(_t_,_vt_, _n_)

#define CX_FIELD_IMP(_t_,_vt_, _n_)                             \
CX_FIELD_GET(_t_,_vt_, _n_)                                     \
CX_FIELD_SET(_t_,_vt_, _n_)

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
        if(_ele_->func != _func_)continue;                      \
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

void  __cxObjectRetain(cxAny ptr);

void  __cxObjectRelease(cxAny ptr);

cxAny __cxObjectAutoRelease(cxAny ptr);

//must completed cxUtilPrint function with platform
void cxUtilPrint(cxConstChars type,cxConstChars file,cxInt line,cxConstChars format,va_list ap);

void cxUtilInfo(cxConstChars file, cxInt line, cxConstChars format, ...);

void cxUtilError(cxConstChars file, cxInt line, cxConstChars format, ...);

void cxUtilWarn(cxConstChars file, cxInt line, cxConstChars format, ...);

void cxUtilAssert(cxConstChars file, cxInt line, cxConstChars format, ...);


//////////////////////////////////////////////////////////
#define CX_NAME_MAX_SIZE  64
typedef struct cxType *cxType;

typedef struct cxMethod *cxMethod;

typedef struct cxSignature *cxSignature;

typedef struct cxProperty *cxProperty;
// propertys
#define CX_SETTER_DEF(_t_,_p_)          static void __##_t_##_p_##Setter(_t_ this, cxConstChars pkey, cxAny value)

#define CX_GETTER_DEF(_t_,_p_)          static cxAny __##_t_##_p_##Getter(_t_ this, cxConstChars pkey)

#define CX_SETTER(_t_,_p_)              do{cxPropertyNew(this,#_p_)->cxSetter=__##_t_##_p_##Setter;}while(0)

#define CX_GETTER(_t_,_p_)              do{cxPropertyNew(this,#_p_)->cxGetter=__##_t_##_p_##Getter;}while(0)

struct cxProperty {
    cxAny data;
    cxAny cxSetter;
    cxAny cxGetter;
    cxChar name[CX_NAME_MAX_SIZE];
    UT_hash_handle hh;
};

cxBool cxPropertyRunSetter(cxAny object,cxConstChars key,cxAny value);

cxBool cxPropertyRunGetter(cxAny object,cxConstChars key,cxAny *value);

cxProperty cxPropertyNew(cxType ptype,cxConstChars name);

//methods def
#define CX_M(_rv_,...)                  (_rv_ (*)(cxAny,##__VA_ARGS__))

#define CX_CALL(_o_,_n_,_m_,...)        (_m_(cxMethodGet(_o_,#_n_)))(_o_,##__VA_ARGS__)

#define CX_SUPER(_b_,_o_,_n_,_m_,...)   (_m_(cxMethodSuper(_o_,_b_##TypeName,#_n_)))(_o_,##__VA_ARGS__)

#define CX_METHOD_DEF(_t_,_n_,_rv_,...) static _rv_ __##_t_##_##_n_(_t_ this,##__VA_ARGS__)

#define CX_METHOD_GET(_o_,_n_)          cxMethodGet(_o_,#_n_)

#define CX_METHOD(_t_,_n_)              do{cxMethodNew(this,#_n_)->cxMethodFunc = (cxAny)__##_t_##_##_n_;}while(0)

#define CX_METHOD_HAS(_o_,_n_)          cxMethodHas(_o_,#_n_)

#define CX_METHOD_EXISTS(_t_,_n_)       cxMethodExists(_t_##TypeName,#_n_)

#define CX_METHOD_RUN(_o_,_n_,_m_,...)  if(CX_METHOD_HAS(_o_,_n_))CX_CALL(_o_,_n_,_m_,##__VA_ARGS__)

//delegate def
#define CX_DELEGATE_DEF(_t_,_s_,_n_,_rv_,...)   static _rv_ __##_t_##_##_s_##_##_n_(_t_ this,##__VA_ARGS__)

#define CX_DELEGATE(_t_,_s_,_n_)                do{cxMethodNew(this,#_s_"_"#_n_)->cxMethodFunc = (cxAny)__##_t_##_##_s_##_##_n_;}while(0)

#define CX_DELEGATE_CALL(_o_,_s_,_n_,_m_,...)   CX_CALL(_o_,_s_##_##_n_,_m_,##__VA_ARGS__)

#define CX_DELEGATE_RUN(_o_,_s_,_n_,_m_,...)    if(CX_METHOD_HAS(_o_,_s_##_##_n_))CX_CALL(_o_,_s_##_##_n_,_m_,##__VA_ARGS__)

struct cxMethod {
    cxAny data;
    cxAny cxMethodFunc;
    cxChar name[CX_NAME_MAX_SIZE];
    UT_hash_handle hh;
};

cxMethod cxMethodNew(cxType ptype,cxConstChars name);

cxBool cxMethodExists(cxConstType type,cxConstChars name);

cxBool cxMethodHas(cxAny object,cxConstChars key);

cxAny cxMethodGet(cxAny pobj,cxConstChars name);

cxAny cxMethodSuper(cxAny pobj,cxConstType type,cxConstChars name);
//signature
struct cxSignature{
    cxChar name[CX_NAME_MAX_SIZE];
    UT_hash_handle hh;
};

cxSignature cxSignatureNew(cxType ptype,cxConstChars name);

cxBool cxSignatureHas(cxSignature this,cxConstChars name);
//types
struct cxType {
    cxConstType typeName;
    cxInt typeSizeof;
    cxAny (*Create)();
    cxAny (*Alloc)();
    cxType superType;
    cxProperty propertys;
    cxMethod methods;
    cxSignature signatures;
    cxChar name[CX_NAME_MAX_SIZE];
    cxChars UI;                     //bind ui json file
    UT_hash_handle hh;
};

cxType cxTypeNew(cxConstType ct,cxConstType sb,cxAny (*create)(),cxAny (*alloc)(),void (*autoType)(cxType),cxInt typeSizeof);

cxType cxTypesGetType(cxConstType name);

cxAny cxTypesCreateObject(cxConstType name);

cxAny cxTypesAllocObject(cxConstType name);

void cxTypeBindUI(cxType this,cxConstChars path);

#define CX_BIND_UI(_f_) cxTypeBindUI(this,_f_)

cxProperty cxTypeGetProperty(cxType this,cxConstChars name);

cxMethod cxTypeGetMethod(cxType this,cxConstChars name);

//check pointer within pobi memory
cxBool cxValidPointer(cxAny pobj,cxAny pointer);

cxBool cxInstanceOf(cxAny pobj,cxConstType type);
//////////////////////////////////////////////////////////

cxDouble cxTimestamp();

typedef enum {
    cxObjectAttrNone    = 0,
    cxObjectAttrMessage = 1 << 0,//need remove message
}cxObjectAttr;

//base type define
CX_BEG(cxObject,cxObject)
    cxType cxType;
    cxInt cxRefcount;
    void(*cxAutoFree)(cxAny);
    CX_FIELD_DEF(cxAny UserData);
    CX_FIELD_DEF(cxObjectAttr Attr);
CX_END(cxObject,cxObject)

CX_INLINE void cxObjectSetAttr(cxAny pthis,const cxObjectAttr value)
{
    CX_ASSERT_THIS(pthis, cxObject);
    this->Attr |= value;
}
CX_FIELD_IMP(cxObject, cxAny, UserData);

cxAny cxObjectSerialize(cxAny pobj);

void cxCoreInit();

void cxCoreClear();

void cxCoreFree();
//reand json from src
CX_EXTERN cxAny cxJsonLoader(cxConstChars src);
//get json from localize key
//return json_t *
CX_EXTERN cxAny cxJsonLocalized(cxConstChars key);
//get json from from poperty
//return json_t *
CX_EXTERN cxAny cxJsonProperty(cxConstChars key);
//type register completed
CX_EXTERN void cxTypeFinished(cxType this);

#endif











