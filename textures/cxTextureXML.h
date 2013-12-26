//
//  cxTextureXML.h
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTextureXML_h
#define cxEngine_cxTextureXML_h

#include <core/cxTexture.h>
#include <core/cxStream.h>
#include <core/cxHash.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTextureXML, cxTexture)
    cxTexture innerTexture;
CX_OBJECT_END(cxTextureXML)

cxTexture cxTextureXMLLoadStream(cxStream stream);

CX_C_END

#endif
