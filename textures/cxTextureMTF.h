//
//  cxTextureMTF.h
//  cxEngineStatic
//  texture set
//  Created by xuhua on 2/25/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineStatic_cxTextureMTF_h
#define cxEngineStatic_cxTextureMTF_h

#include <lzma/cxLzma.h>
#include <engine/cxTexture.h>

CX_C_BEGIN

CX_DEF(cxTextureMTF, cxTexture)
    cxTextureMTFHeader header;
    cxHash caches;
CX_END(cxTextureMTF, cxTexture)

cxTexture cxTextureMTFCreate(cxConstChars file);

cxTexture cxTextureMTFGet(cxAny pthis,cxInt idx);

CX_C_END

#endif
