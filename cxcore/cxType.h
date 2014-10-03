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

CX_C_BEGIN

CX_OBJECT_DEF(cxType, cxObject)
    cxAny (*Create)();
    cxAny (*Alloc)();
    cxConstType typeName;
    cxString signature;    //a.b.c
    cxType superType;
    cxHash properties;
CX_OBJECT_END(cxType, cxObject)

typedef cxJson (*cxObjectCreateReaderFunc)(cxConstChars src);

cxType cxTypesGet(cxConstType type);

cxProperty cxTypeSetProperty(cxType this,cxConstChars key);

cxProperty cxTypeProperty(cxType this,cxConstChars key);

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








