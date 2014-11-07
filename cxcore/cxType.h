//
//  cxInitType.h
//  cxCore
//  system key: cxId cxType cxSrc
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxType_h
#define cxCore_cxType_h

#include "cxHash.h"
#include "cxJson.h"
#include "cxProperty.h"
#include "cxString.h"
#include "cxMethod.h"

CX_C_BEGIN

CX_DEF(cxType, cxObject)
    cxAny (*Create)();
    cxAny (*Alloc)();
    cxConstType typeName;
    cxString signature;   //类型签名 类似:a.b.c
    cxType superType;
    cxHash properties;
    cxHash methods;
CX_END(cxType, cxObject)

typedef cxJson (*cxObjectCreateReaderFunc)(cxConstChars src);

void cxTypeCopyMethods(cxType this,cxType super);

void cxTypeCopyPropertys(cxType this,cxType super);

cxType cxTypesGet(cxConstType type);

cxAny cxJsonTocxObject(cxJson v);

cxMethod cxTypeGetMethod(cxType this,cxConstChars key);

cxMethod cxTypeSetMethod(cxType this,cxConstChars key);

cxProperty cxTypeSetProperty(cxType this,cxConstChars key);

cxProperty cxTypeGetProperty(cxType this,cxConstChars key);

void cxTypeSignature(cxType type,cxType super);

void cxTypeSetSuper(cxType type,cxType super);

void cxTypesSet(cxConstType typeName,cxType type);

void cxTypeRunObjectSetter(cxObject object,cxJson json);
//
cxAny cxObjectCreateUseType(cxConstType type);
//use end set object property
cxAny cxObjectCreateUseJson(cxJson json);

void cxTypesInit();

void cxTypesFree();

CX_C_END

#endif








