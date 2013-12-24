//
//  cxAssetsLuaImp.c
//  cxEngineIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxAssetsStream.h"

const luaL_Reg cxAssetsStreamInstanceMethods[] = {
    
    CX_LUA_SUPER(cxStream)
};

const luaL_Reg cxAssetsStreamTypeMethods[] = {
    CX_LUA_TYPE(cxAssetsStream)
};

void cxAssetsStreamTypeInit()
{
    CX_LUA_LOAD_TYPE(cxAssetsStream);
}