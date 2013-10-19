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
    cxNumberTypeDouble
}cxNumberType;

typedef union {
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
}cxNumberValue;

CX_OBJECT_DEF(cxNumber, cxObject)
    cxNumberValue value;
    cxNumberType type;
CX_OBJECT_END(cxNumber)

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














