//
//  cxPlayerLuaImp.c
//  cxEngineIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxPlayer.h"

const luaL_Reg cxPlayerInstanceMethods[] = {
    CX_LUA_SUPER(cxObject)
};

const luaL_Reg cxPlayerTypeMethods[] = {
    CX_LUA_TYPE(cxPlayer)
};

void cxPlayerTypeInit()
{
    CX_LUA_LOAD_TYPE(cxPlayer);
}