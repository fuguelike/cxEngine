//
//  cxTexturePKM.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTexturePKM.h"

static cxBool cxTexturePKMLoad(cxAny this,cxStream stream)
{
    return false;
}

static void cxTexturePKMBind(cxAny this)
{
    
}

static const cxTextureInterface pkmInterface = {
    .Load = cxTexturePKMLoad,
    .Bind = cxTexturePKMBind,
};

CX_OBJECT_INIT(cxTexturePKM, cxTexture)
{
    this->super.interface = &pkmInterface;
}
CX_OBJECT_FREE(cxTexturePKM, cxTexture)
{
    //
}
CX_OBJECT_TERM(cxTexturePKM, cxTexture)

cxTexture cxTexturePKMLoadStream(cxStream stream)
{
    cxTexturePKM pkm = CX_CREATE(cxTexturePKM);
    cxTextureLoad((cxTexture)pkm,stream);
    return (cxTexture)pkm;
}
