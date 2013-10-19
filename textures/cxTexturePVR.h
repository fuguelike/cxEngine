//
//  cxTexturePVR.h
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTexturePVR_h
#define cxEngine_cxTexturePVR_h

#include <core/cxTexture.h>
#include <core/cxStream.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTexturePVR, cxTexture)
    GLenum format;
    GLenum glFormat;
CX_OBJECT_END(cxTexturePVR)

cxTexture cxTexturePVRLoadStream(cxStream stream);

CX_C_END

#endif
