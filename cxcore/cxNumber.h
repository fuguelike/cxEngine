//
//  cxNumber.h
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxNumber_h
#define cxEngine_cxNumber_h

#include <cxcore/cxCore.h>

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
}cxNumberValue;

CX_OBJECT_DEF(cxNumber, cxObject)
    cxNumberValue value;
    cxNumberType type;
CX_OBJECT_END(cxNumber, cxObject)

cxBool cxNumberIsType(cxAny pnum,cxNumberType type);

cxBool cxNumberToBool(cxAny pnum);

cxInt cxNumberToInt(cxAny pnum);

cxInt8 cxNumberToInt8(cxAny pnum);

cxInt16 cxNumberToInt16(cxAny pnum);

cxInt32 cxNumberToInt32(cxAny pnum);

cxInt64 cxNumberToInt64(cxAny pnum);

cxUInt cxNumberToUInt(cxAny pnum);

cxUInt8 cxNumberToUInt8(cxAny pnum);

cxUInt16 cxNumberToUInt16(cxAny pnum);

cxUInt32 cxNumberToUInt32(cxAny pnum);

cxUInt64 cxNumberToUInt64(cxAny pnum);

cxFloat cxNumberToFloat(cxAny pnum);

cxDouble cxNumberToDouble(cxAny pnum);

cxNumber cxNumberBool(cxBool v);

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














