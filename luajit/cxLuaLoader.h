//
//  cxLuaLoader.h
//  cxEngine
//
//  Created by xuhua on 12/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLuaLoader_h
#define cxEngine_cxLuaLoader_h

#include <core/cxBase.h>
#include <core/cxString.h>
#include <luajit/lauxlib.h>
#include <luajit/lualib.h>
#include <luajit/luajit.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxLuaLoader, cxObject)
    lua_State *L;
CX_OBJECT_END(cxLuaLoader)

CX_C_END

#endif
