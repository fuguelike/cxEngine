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
#include <engine/cxStream.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTextureTXT, cxTexture)
    cxString font;
    cxString string;
    cxTextAttr attr;
CX_OBJECT_END(cxTextureTXT, cxTexture)

void cxTextureTXTMakeTexture(cxTextureTXT texture,cxAny buffer,cxInt width,cxInt height);

CX_C_END

#endif
