//
//  cxScript.h
//  cxEngine
//
//  Created by xuhua on 11/30/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxScript_h
#define cxEngine_cxScript_h

#include <luajit/lualib.h>
#include <luajit/lauxlib.h>
#include <core/cxBase.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxScript, cxObject)
    lua_State *L;
CX_OBJECT_END(cxScript)

cxBool cxScriptAppend(cxAny ps,cxString code);

cxBool cxScriptCallFunc(cxAny ps,cxInt an,cxInt rn);

cxBool cxScriptCheckFunc(cxAny ps,cxConstChars name);

cxBool cxScriptInit(cxAny ps);

cxBool cxScriptMain(cxAny ps);

cxBool cxScriptFree(cxAny ps);

cxBool cxScriptLoad(cxAny ps,cxString code);

CX_C_END

#endif
