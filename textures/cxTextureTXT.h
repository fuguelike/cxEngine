//
//  cxTextureTXT.h
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTextureTXT_h
#define cxEngine_cxTextureTXT_h

#include <core/cxTexture.h>
#include <core/cxStream.h>
#include <core/cxHash.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTextureTXT, cxTexture)
    cxString fontfile;
    cxString string;
    cxTextAttr attr;
CX_OBJECT_END(cxTextureTXT)

void cxTextureTXTMakeTexture(cxTextureTXT texture,cxPointer buffer,cxInt width,cxInt height);

CX_C_END

#endif
