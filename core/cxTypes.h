//
//  cxTypes.h
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTypes_h
#define cxEngine_cxTypes_h

#include <utarray.h>
#include "cxBase.h"

CX_C_BEGIN

typedef enum {
    cxTypesString,
    cxTypesDB,
    cxTypesHash,
    cxTypesArray,
    cxTypesNumber,
}cxTypesType;

#define cxTypesIsType(o,t)  ((o) != NULL && (o)->type == t)

CX_OBJECT_DEF(cxTypes, cxObject)
    cxTypesType type;
    cxAny any;
CX_OBJECT_END(cxTypes)

cxTypes cxDBTypesCreate(cxAny db);

cxTypes cxHashTypesCreate();

cxTypes cxNumberTypesCreate();

cxTypes cxArrayTypesCreate();

cxTypes cxStringTypesCreate();

#define cxTypesLength(a)        utarray_len((a)->utArray)

#define cxTypesAppend(a,v)      utarray_push_back((a)->utArray,&(v))

#define cxTypesIndex(a,t,i)     (*((t *)utarray_eltptr((a)->utArray,i)))

#define CX_TYPES_FOREACH(a,t,e) \
t *e = NULL; \
while((a) != NULL && (e = (t *)utarray_next((a)->utArray, e)) != NULL)

void cxLuaPushVec2fv(lua_State *L,cxVec2f pos);

void cxLuaPushSize2fv(lua_State *L,cxSize2f size);

void cxLuaPushColor4f(lua_State *L,cxColor4f color);

cxColor4f cxLuaColor4fValue(lua_State *L,cxInt n,cxColor4f d);

cxVec2f cxVec2fValue(cxVec2fRange rv);

cxSize2f cxLuaSize2fValue(lua_State *L,cxInt n,cxSize2f d);

cxVec2f cxLuaVec2fValue(lua_State *L,cxInt n,cxVec2f d);

cxAny cxLuaObjectValue(lua_State *L,cxInt n);

cxBool cxLuaBoolValue(lua_State *L,cxInt n,cxBool d);

cxInt cxLuaIntValue(lua_State *L,cxInt n,cxInt d);

cxConstChars cxLuaStringValue(lua_State *L,cxInt n,cxConstChars d);

cxFloat cxLuaFloatValue(lua_State *L,cxInt n,cxFloat d);

CX_C_END

#endif














