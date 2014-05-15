//
//  cxInitType.h
//  cxEngine
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxInitType_h
#define cxEngine_cxInitType_h

#include "cxBase.h"
#include "cxJson.h"
#include "cxHash.h"
#include "cxProperty.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxType, cxObject)
    CX_METHOD_DEF(cxAny, Create);
    CX_METHOD_DEF(cxAny, Alloc);
    cxConstType typeName;
    cxType superType;
    cxHash properties;
CX_OBJECT_END(cxType)

#define CX_REGISTER_TYPE(_t_,_b_)                           \
do{                                                         \
    cxType superType =  cxTypeGet(#_b_);                    \
    CX_ASSERT(superType != NULL,"super not register");      \
    cxType _tmp_ = CX_ALLOC(cxType);                        \
    cxTypeSetSuper(_tmp_,superType);                        \
    CX_METHOD_SET(_tmp_->Alloc, __##_t_##AllocFunc);        \
    CX_METHOD_SET(_tmp_->Create, __##_t_##CreateFunc);      \
    cxTypeSet(_t_##TypeName,_tmp_);                         \
    __##_t_##InitType(_tmp_);                               \
    CX_RELEASE(_tmp_);                                      \
}while(0)

void cxTypeInvokeSetter(cxType this,cxConstChars key,cxAny object,cxAny value);

void cxTypeRunObjectSetter(cxObject object,cxJson json);

cxAny cxTypeInvokeGetter(cxType this,cxConstChars key,cxAny object);

cxAny cxObjectLoadWithJson(cxJson json);

cxAny cxObjectLoadWithFile(cxConstChars file);

cxAny cxTypeCreate(cxJson json);

cxType cxTypeGet(cxConstType type);

void cxTypeSetSuper(cxType type,cxType super);

void cxTypeSet(cxConstType typeName,cxType type);

cxProperty cxTypeProperty(cxType this,cxConstChars key);

void cxTypeInit();

void cxTypeFree();

CX_C_END

#endif








