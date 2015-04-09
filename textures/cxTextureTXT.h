//
//  cxTextureTXT.h
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTextureTXT_h
#define cxEngine_cxTextureTXT_h

#include <engine/cxTexture.h>
#include <cxcore/cxStream.h>

CX_C_BEGIN

CX_DEF(cxTextureTXT, cxTexture)
    cxStr font;
    cxStr string;
    cxTextAttr attr;
CX_END(cxTextureTXT, cxTexture)

void cxTextureTXTMakeTexture(cxTextureTXT texture,cxAny buffer,cxInt width,cxInt height);

CX_C_END

#endif
