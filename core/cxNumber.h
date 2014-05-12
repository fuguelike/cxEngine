//
//  cxNumber.h
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxNumber_h
#define cxEngine_cxNumber_h

#include "cxBase.h"

CX_C_BEGIN

typedef enum {
    cxNumberTypeNone = 0,
    cxNumberTypeBool,
    cxNumberTypeInt,
    cxNumberTypeInt8,
    cxNumberTypeInt16,
    cxNumberTypeInt32,
    cxNumberTypeInt64,
    cxNumberTypeUInt,
    cxNumberTypeUInt8,
    cxNumberTypeUInt16,
    cxNumberTypeUInt32,
    cxNumberTypeUInt64,
    cxNumberTypeFloat,
    cxNumberTypeDouble,
    cxNumberTypeVec2f,
    cxNumberTypeVec2i,
    cxNumberTypeSize2f,
    cxNumberTypeTex2f,
    cxNumberTypeSize2i,
}cxNumberType;

typedef union {
    cxBool bv;
    cxInt vi;
    cxInt8  i8;
    cxInt16 i16;
    cxInt32 i32;
    cxInt64 i64;
    cxUInt vu;
    cxUInt8 u8;
    cxUInt16 u16;
    cxUInt32 u32;
    cxUInt64 u64;
    cxFloat vf;
    cxDouble vd;
    cxVec2f vec2f;
    cxVec2i vec2i;
    cxSize2f size2f;
    cxTex2f tex2f;
    cxSize2i size2i;
}cxNumberValue;

CX_OBJECT_DEF(cxNumber, cxObject)
    cxNumberValue value;
    cxNumberType type;
CX_OBJECT_END(cxNumber)

cxBool cxNumberIsType(cxAny this,cxNumberType type);

cxBool cxNumberToBool(cxNumber this);

cxVec2i cxNumberToVec2i(cxNumber this);

cxVec2f cxNumberToVec2f(cxNumber this);

cxTex2f cxNumberToTex2f(cxNumber this);

cxSize2f cxNumberToSize2f(cxNumber this);

cxSize2i cxNumberToSize2i(cxNumber this);

cxInt cxNumberToInt(cxNumber this);

cxInt8 cxNumberToInt8(cxNumber this);

cxInt16 cxNumberToInt16(cxNumber this);

cxInt32 cxNumberToInt32(cxNumber this);

cxInt64 cxNumberToInt64(cxNumber this);

cxUInt cxNumberToUInt(cxNumber this);

cxUInt8 cxNumberToUInt8(cxNumber this);

cxUInt16 cxNumberToUInt16(cxNumber this);

cxUInt32 cxNumberToUInt32(cxNumber this);

cxUInt64 cxNumberToUInt64(cxNumber this);

cxFloat cxNumberToFloat(cxNumber this);

cxDouble cxNumberToDouble(cxNumber this);

cxNumber cxNumberBool(cxBool v);

cxNumber cxNumberVec2i(cxVec2i v);

cxNumber cxNumberVec2f(cxVec2f v);

cxNumber cxNumberTex2f(cxTex2f v);

cxNumber cxNumberSize2f(cxSize2f v);

cxNumber cxNumberSize2i(cxSize2i v);

cxNumber cxNumberInt(cxInt v);

cxNumber cxNumberInt8(cxInt8 v);

cxNumber cxNumberInt16(cxInt16 v);

cxNumber cxNumberInt32(cxInt32 v);

cxNumber cxNumberInt64(cxInt64 v);

cxNumber cxNumberUInt(cxUInt v);

cxNumber cxNumberUInt8(cxUInt8 v);

cxNumber cxNumberUInt16(cxUInt16 v);

cxNumber cxNumberUInt32(cxUInt32 v);

cxNumber cxNumberUInt64(cxUInt64 v);

cxNumber cxNumberFloat(cxFloat v);

cxNumber cxNumberDouble(cxDouble v);

CX_C_END

#endif














