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

typedef enum {
    cxBaseTypeObject,
    cxBaseTypeView,
    cxBaseTypeAction,
    cxBaseTypeStream,
    cxBaseTypeTexture,
    cxBaseTypeShader,
}cxBaseType;

typedef void (*cxObjectFunc)(cxPointer this);

typedef cxAny (*cxAnyFunc)(cxAny object);

cxAny cxObjectAlloc(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

cxAny cxObjectCreate(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

void cxObjectRetain(cxAny ptr);

void cxObjectRelease(cxAny ptr);

cxAny cxObjectAutoRelease(cxAny ptr);

cxBool cxObjectIsBaseType(cxAny pobj,cxBaseType type);

cxBool cxObjectIsType(cxAny pobj,cxConstType type);

cxConstType cxObjectType(cxAny pobj);

cxAny cxObjectRoot(cxAny obj);

void cxObjectSetRoot(cxAny obj,cxAny root);

void cxObjectSetTag(cxAny obj,cxInt tag);

cxInt cxObjectTag(cxAny obj);

#define CX_RETURN(cond,...)         if(cond)return __VA_ARGS__

#define CX_BREAK(cond)              if(cond)break

#define CX_CONTINUE(cond)           if(cond)continue

#define CX_OBJECT_DEF(_t_,_b_)      CX_OBJECT_BEG(_t_) struct _b_ super;

#define CX_OBJECT_INIT(_t_,_b_)     void __##_t_##AutoInit(_t_ this){__##_b_##AutoInit((_b_)this);{

#define CX_OBJECT_FREE(_t_,_b_)     }};void __##_t_##AutoFree(_t_ this){{

#define CX_OBJECT_TERM(_t_,_b_)     }__##_b_##AutoFree((_b_)this);}

#define CX_ALLOC(_t_)               cxObjectAlloc(_t_##TypeName,sizeof(struct _t_),(cxObjectFunc)__##_t_##AutoInit,(cxObjectFunc)__##_t_##AutoFree)

#define CX_CREATE(_t_)              cxObjectCreate(_t_##TypeName,sizeof(struct _t_),(cxObjectFunc)__##_t_##AutoInit,(cxObjectFunc)__##_t_##AutoFree)

#define CX_RETAIN(_o_)              cxObjectRetain(_o_)

#define CX_RELEASE(_o_)             cxObjectRelease(_o_)

#define CX_AUTOFREE(_o_)            cxObjectAutoRelease(_o_)

#define CX_RETAIN_SWAP(_s_,_d_)     {CX_RELEASE(_s_);(_s_)=(cxAny)(_d_);CX_RETAIN(_s_);}

#define CX_RETAIN_SET(_n_,_v_)      {_n_ = _v_;CX_RETAIN(_n_);}

//object

#define CX_OBJECT_BEG(_t_)                                                  \
static CX_UNUSED_ATTRIBUTE cxConstType _t_##TypeName = #_t_;                \
typedef struct _t_ *_t_;                                                    \
void __##_t_##AutoInit(_t_ this);                                           \
void __##_t_##AutoFree(_t_ this);                                           \
struct _t_ {

#define CX_OBJECT_END(_t_) };

//method

#define CX_METHOD_ALLOC(_r_,_n_,...)    _r_ (*_n_)(__VA_ARGS__)

#define CX_METHOD_RELEASE(_m_)          _m_ = NULL

#define CX_METHOD_GET(_d_,_m_,...)      (((_m_) != NULL)?((_m_)(__VA_ARGS__)):(_d_))

#define CX_METHOD_RUN(_m_,...)          if((_m_) != NULL)(_m_)(__VA_ARGS__)

#define CX_METHOD_OVERRIDE(_m_,_f_)     _m_ = _f_

//base type define
CX_OBJECT_BEG(cxObject)
    cxConstType cxType;
    cxBaseType cxBase;
    cxUInt cxRefcount;
    cxObjectFunc cxFree;
    cxInt cxTag;
CX_OBJECT_END(cxObject)

CX_OBJECT_DEF(cxMemory, cxObject)
    cxPointer pointer;
CX_OBJECT_END(cxMemory)

cxMemory cxMemoryCreate(cxInt size);

CX_C_END

#endif
