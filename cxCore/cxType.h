//
//  cxInitType.h
//  cxCore
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxInitType_h
#define cxCore_cxInitType_h

#include "cxBase.h"
#include "cxHash.h"
#include "cxProperty.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxType, cxObject)
    cxAny (*Create)();
    cxAny (*Alloc)();
    cxConstType typeName;
    cxType superType;
    cxHash properties;
CX_OBJECT_END(cxType)

#define CX_REGISTER_TYPE(_t_,_b_)                           \
do{                                                         \
    cxType superType =  cxTypesGet(#_b_);                   \
    CX_ASSERT(superType != NULL,"super not register");      \
    cxType _tmp_ = CX_ALLOC(cxType);                        \
    cxTypeSetSuper(_tmp_,superType);                        \
    _tmp_->Alloc = __##_t_##AllocFunc;                      \
    _tmp_->Create = __##_t_##CreateFunc;                    \
    cxTypesSet(_t_##TypeName,_tmp_);                        \
    __##_t_##AutoType(_tmp_);                               \
    CX_RELEASE(_tmp_);                                      \
}while(0)

cxAny cxTypeCreate(cxConstType type);

cxType cxTypesGet(cxConstType type);

cxProperty cxTypeProperty(cxType this,cxConstChars key);

void cxTypeSetSuper(cxType type,cxType super);

cxBool cxInstanceOf(cxAny object,cxConstType type);

void cxTypesSet(cxConstType typeName,cxType type);

void cxTypesInit();

void cxTypesFree();

CX_C_END

#endif








