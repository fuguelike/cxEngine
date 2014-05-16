//
//  cxTextureXML.h
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTextureJSON_h
#define cxEngine_cxTextureJSON_h

#include <engine/cxTexture.h>
#include <engine/cxStream.h>

CX_C_BEGIN

//use texturepack to json array file

CX_OBJECT_DEF(cxTextureJSON, cxTexture)
    cxTexture innerTexture;
CX_OBJECT_END(cxTextureJSON)

cxTexture cxTextureJSONLoadStream(cxStream stream);

CX_C_END

#endif
