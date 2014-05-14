//
//  cxObject.h
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxObject_h
#define cxEngine_cxObject_h

#include "cxConfig.h"
#include "cxAllocator.h"

CX_C_BEGIN

//
typedef void (*cxObjectFunc)(cxAny);

//this,json,hash
typedef void (*cxObjectInit)(cxAny,cxAny,cxAny);

//
typedef cxAny (*cxAnyFunc)(cxAny object);

cxAny cxObjectAlloc(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

cxAny cxObjectCreate(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

void cxObjectRetain(cxAny ptr);

void cxObjectRelease(cxAny ptr);

cxAny cxObjectAutoRelease(cxAny ptr);

cxBool cxInstanceOf(cxAny object,cxConstType type);

//object

#define CX_OBJECT_BEG(_t_,...)                                  \
CX_ATTRIBUTE_UNUSED static cxConstType _t_##TypeName=#_t_;      \
typedef struct _t_ *_t_;                                        \
void __##_t_##AutoInit(_t_ this);                               \
void __##_t_##AutoFree(_t_ this);                               \
void __##_t_##InitObject(cxAny,cxAny,cxAny);                    \
struct _t_ {

#define CX_OBJECT_END(_t_) };                                   \
CX_ATTRIBUTE_UNUSED static cxAny __##_t_##CreateFunc()          \
{                                                               \
    return CX_CREATE(_t_);                                      \
}                                                               \
CX_ATTRIBUTE_UNUSED static cxAny __##_t_##AllocFunc()           \
{                                                               \
    return CX_ALLOC(_t_);                                       \
}

#define CX_RETURN(cond,...)         if(cond)return __VA_ARGS__

#define CX_BREAK(cond)              if(cond)break

#define CX_CONTINUE(cond)           if(cond)continue

#define CX_OBJECT_DEF(_t_,_b_)      CX_OBJECT_BEG(_t_,_b_) struct _b_ super;

#define CX_OBJECT_INIT(_t_,_b_)     void __##_t_##AutoInit(_t_ this){__##_b_##AutoInit((_b_)this);{

#define CX_OBJECT_FREE(_t_,_b_)     }};void __##_t_##AutoFree(_t_ this){{

#define CX_OBJECT_TERM(_t_,_b_)     }__##_b_##AutoFree((_b_)this);}

#define CX_ALLOC(_t_)               cxObjectAlloc(_t_##TypeName,sizeof(struct _t_),(cxObjectFunc)__##_t_##AutoInit,(cxObjectFunc)__##_t_##AutoFree)

#define CX_CREATE(_t_)              cxObjectCreate(_t_##TypeName,sizeof(struct _t_),(cxObjectFunc)__##_t_##AutoInit,(cxObjectFunc)__##_t_##AutoFree)

#define CX_RETAIN(_o_)              cxObjectRetain(_o_)

#define CX_RELEASE(_o_)             cxObjectRelease(_o_)

//只在绘制线程中试用
#define CX_AUTOFREE(_o_)            cxObjectAutoRelease(_o_)

#define CX_RETAIN_SWAP(_s_,_d_)     {CX_RELEASE(_s_);(_s_)=(cxAny)(_d_);CX_RETAIN(_s_);}

#define CX_RETAIN_SET(_n_,_v_)      {_n_ = _v_;CX_RETAIN(_n_);}

#define CX_INSTANCE_OF(_o_,_t_)     cxInstanceOf(_o_,_t_##TypeName)

#define CX_CAST(_t_,_o_)            (_t_)(_o_);if(!CX_INSTANCE_OF(_o_,_t_))CX_ASSERT_FALSE("cast to type(%s) error",#_t_)

//method

#define CX_METHOD_DEF(_r_,_n_,...)      _r_ (*_n_)(__VA_ARGS__)

#define CX_METHOD_GET(_d_,_m_,...)      (((_m_) != NULL)?((_m_)(__VA_ARGS__)):(_d_))

#define CX_METHOD_RUN(_m_,...)          if((_m_) != NULL)(_m_)(__VA_ARGS__)

#define CX_METHOD_SET(_m_,_f_)          _m_ = _f_

//base type define
CX_OBJECT_BEG(cxObject)
    cxConstType cxType;
    cxUInt cxRefcount;
    cxObjectFunc cxFree;
    cxObjectInit cxInit;
CX_OBJECT_END(cxObject)

//invoke super init method
#define CX_OBJECT_INIT_SUPER(_t_)       __##_t_##InitObject(object, json, hash)

//override init method
#define CX_OBJECT_INIT_OVERRIDE(_t_)    CX_METHOD_SET(((cxObject)this)->cxInit, __##_t_##InitObject)

CX_OBJECT_DEF(cxMemory, cxObject)
    cxPointer pointer;
CX_OBJECT_END(cxMemory)

cxMemory cxMemoryCreate(cxInt size);

CX_C_END

#endif
