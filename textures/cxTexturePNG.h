//
//  cxTexturePNG.h
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTexturePNG_h
#define cxEngine_cxTexturePNG_h

#include <engine/cxTexture.h>
#include <cxcore/cxStream.h>
#include <libpng/png.h>

CX_C_BEGIN

CX_DEF(cxTexturePNG, cxTexture)

CX_END(cxTexturePNG, cxTexture)

cxTexture cxTexturePNGLoadStream(cxStream stream);

CX_C_END

#endif
