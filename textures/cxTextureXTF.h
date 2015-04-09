//
//  cxTextureXTF.h
//  cxEngine
//  xuhua texture format
//  Created by xuhua on 11/29/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTextureXTF_h
#define cxEngine_cxTextureXTF_h

#include <engine/cxTexture.h>

CX_C_BEGIN

CX_DEF(cxTextureXTF, cxTexture)
    cxArray textures;
CX_END(cxTextureXTF, cxTexture)

cxTexture cxTextureXTFLoadStream(cxStream stream);

CX_C_END

#endif
