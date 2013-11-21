//
//  cxTexturePKM.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTexturePKM.h"

typedef struct{
	uint32_t pkmTag;
    uint16_t version;
    uint16_t type;
    uint16_t extWidth;
    uint16_t extHeight;
    uint16_t orgWidth;
    uint16_t orgHeight;
} cxPKMHeader;

#define CX_PKM_TAG (*(uint32_t *)("PKM\x20"))

static cxBool cxTexturePKMLoad(cxAny this,cxStream stream)
{
    return false;
}

static void cxTexturePKMBind(cxAny this)
{
    
}

CX_OBJECT_INIT(cxTexturePKM, cxTexture)
{
    CX_METHOD_SET(this->super.Bind, cxTexturePKMBind);
    CX_METHOD_SET(this->super.Load, cxTexturePKMLoad);
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
