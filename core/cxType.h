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

CX_C_BEGIN

CX_OBJECT_DEF(cxType, cxObject)
    CX_METHOD_ALLOC(cxAny, Create);
    CX_METHOD_ALLOC(cxAny, Alloc);
    cxConstType typeName;
    cxType superType;
CX_OBJECT_END(cxType)

#define CX_REGISTER_TYPE(_t_,_b_)                               \
{                                                               \
    cxType superType =  cxTypeGet(#_b_);                        \
    CX_ASSERT(superType != NULL,"super not register");          \
    cxType _t_##Type = CX_ALLOC(cxType);                        \
    CX_RETAIN_SWAP(_t_##Type->superType,superType);             \
    CX_METHOD_OVERRIDE(_t_##Type->Alloc, __##_t_##AllocFunc);   \
    CX_METHOD_OVERRIDE(_t_##Type->Create, __##_t_##CreateFunc); \
    cxTypeSet(_t_##TypeName,_t_##Type);                         \
    _t_##Type->typeName = _t_##TypeName;                        \
    CX_RELEASE(_t_##Type);                                      \
}

cxAny cxObjectLoadByJson(cxJson json, cxHash hash);

cxAny cxObjectLoad(cxConstChars file,cxHash hash);

cxAny cxTypeCreate(cxJson json,cxAny hash);

cxAny cxTypeGet(cxConstType type);

void cxTypeSet(cxConstType type,cxAny ptype);

void cxInitTypes();

void cxTypeInit();

void cxTypeFree();

CX_C_END

#endif








