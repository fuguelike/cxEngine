//
//  cxLuaFuncs.h
//  cxEngine
//
//  Created by xuhua on 12/19/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLuaFuncs_h
#define cxEngine_cxLuaFuncs_h

#include <core/cxBase.h>
#include <core/cxString.h>
#include <luajit/lauxlib.h>
#include <luajit/lualib.h>
#include <luajit/luajit.h>

CX_C_BEGIN

void cxLuaLoaderFuncs(cxAny loader);

CX_C_END

#endif
