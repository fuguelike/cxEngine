//
//  cxTexturePKM.h
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTexturePKM_h
#define cxEngine_cxTexturePKM_h

#include <engine/cxTexture.h>
#include <cxcore/cxStream.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTexturePKM, cxTexture)

CX_OBJECT_END(cxTexturePKM, cxTexture)

cxTexture cxTexturePKMLoadStream(cxStream stream);

CX_C_END

#endif
