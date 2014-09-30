//
//  cxNumber.c
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxNumber.h"

cxBool cxNumberIsType(cxAny this,cxNumberType type)
{
    cxNumber number = this;
    return number != NULL && number->type == type;
}

CX_OBJECT_TYPE(cxNumber, cxObject)
{}
CX_OBJECT_INIT(cxNumber, cxObject)
{}
CX_OBJECT_FREE(cxNumber, cxObject)
{}
CX_OBJECT_TERM(cxNumber, cxObject)
//
void cxNumberSetBool(cxAny pnum,cxBool v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeBool, "type error");
    this->value.bv = v;
}
void cxNumberSetInt(cxAny pnum, cxInt v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt, "type error");
    this->value.vi = v;
}
void cxNumberSetLong(cxAny pnum, cxLong v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeLong, "type error");
    this->value.vl = v;
}
void cxNumberSetInt8(cxAny pnum,cxInt8 v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt8, "type error");
    this->value.i8 = v;
}
void cxNumberSetInt16(cxAny pnum,cxInt16 v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt16, "type error");
    this->value.i16 = v;
}
void cxNumberSetInt32(cxAny pnum,cxInt32 v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt32, "type error");
    this->value.i32 = v;
}
void cxNumberSetInt64(cxAny pnum,cxInt64 v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt64, "type error");
    this->value.i64 = v;
}
void cxNumberSetUInt(cxAny pnum,cxUInt v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt, "type error");
    this->value.vu = v;
}
void cxNumberSetUInt8(cxAny pnum,cxUInt8 v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt8, "type error");
    this->value.u8 = v;
}
void cxNumberSetUInt16(cxAny pnum,cxUInt16 v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt16, "type error");
    this->value.u16 = v;
}
void cxNumberSetUInt32(cxAny pnum,cxUInt32 v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt32, "type error");
    this->value.u32 = v;
}
void cxNumberSetUInt64(cxAny pnum,cxUInt64 v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt64, "type error");
    this->value.u64 = v;
}
void cxNumberSetFloat(cxAny pnum,cxFloat v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeFloat, "type error");
    this->value.vf = v;
}
void cxNumberSetDouble(cxAny pnum,cxDouble v)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeDouble, "type error");
    this->value.vd = v;
}
//

cxBool cxNumberToBool(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeBool, "type error");
    return this->value.bv;
}

cxInt cxNumberToInt(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt, "type error");
    return this->value.vi;
}

cxLong cxNumberToLong(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeLong, "type error");
    return this->value.vl;
}

cxInt8 cxNumberToInt8(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt8, "type error");
    return this->value.i8;
}

cxInt16 cxNumberToInt16(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt16, "type error");
    return this->value.i16;
}

cxInt32 cxNumberToInt32(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt32, "type error");
    return this->value.i32;
}

cxInt64 cxNumberToInt64(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeInt64, "type error");
    return this->value.i64;
}

cxUInt cxNumberToUInt(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt, "type error");
    return this->value.vu;
}

cxUInt8 cxNumberToUInt8(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt8, "type error");
    return this->value.u8;
}

cxUInt16 cxNumberToUInt16(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt16, "type error");
    return this->value.u16;
}

cxUInt32 cxNumberToUInt32(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt32, "type error");
    return this->value.u32;
}

cxUInt64 cxNumberToUInt64(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeUInt64, "type error");
    return this->value.u64;
}

cxFloat cxNumberToFloat(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeFloat, "type error");
    return this->value.vf;
}

cxDouble cxNumberToDouble(cxAny pnum)
{
    CX_ASSERT_THIS(pnum, cxNumber);
    CX_ASSERT(this->type == cxNumberTypeDouble, "type error");
    return this->value.vd;
}

cxNumber cxNumberBool(cxBool v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeBool;
    rv->value.bv = v;
    return rv;
}

cxNumber cxNumberInt(cxInt v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt;
    rv->value.vi = v;
    return rv;
}

cxNumber cxNumberLong(cxLong v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeLong;
    rv->value.vl = v;
    return rv;
}

cxNumber cxNumberInt8(cxInt8 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt8;
    rv->value.i8 = v;
    return rv;
}

cxNumber cxNumberInt16(cxInt16 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt16;
    rv->value.i16 = v;
    return rv;
}

cxNumber cxNumberInt32(cxInt32 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt32;
    rv->value.i32 = v;
    return rv;
}

cxNumber cxNumberInt64(cxInt64 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt64;
    rv->value.i64 = v;
    return rv;
}

cxNumber cxNumberUInt(cxUInt v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt;
    rv->value.vu = v;
    return rv;
}

cxNumber cxNumberUInt8(cxUInt8 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt8;
    rv->value.u8 = v;
    return rv;
}

cxNumber cxNumberUInt16(cxUInt16 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt16;
    rv->value.u16 = v;
    return rv;
}

cxNumber cxNumberUInt32(cxUInt32 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt32;
    rv->value.u32 = v;
    return rv;
}

cxNumber cxNumberUInt64(cxUInt64 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt64;
    rv->value.u64 = v;
    return rv;
}

cxNumber cxNumberFloat(cxFloat v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeFloat;
    rv->value.vf = v;
    return rv;
}

cxNumber cxNumberDouble(cxDouble v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeDouble;
    rv->value.vd = v;
    return rv;
}

