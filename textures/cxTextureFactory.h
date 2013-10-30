//
//  cxTextureFactory.h
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTextureFactory_h
#define cxEngine_cxTextureFactory_h

#include "cxTexturePVR.h"
#include "cxTexturePNG.h"
#include "cxTextureXML.h"
#include "cxTextureTXT.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxTextureFactory, cxObject)
    cxHash caches;
    CX_SLOT_ALLOC(onMemory);
CX_OBJECT_END(cxTextureFactory)

cxTextureFactory cxTextureFactoryInstance();

cxTexture cxTextureCreate(cxConstChars file);

cxTexture cxTextureFactoryLoadFile(cxConstChars file);

cxTexture cxTextureLoadText(const cxString txt,const cxString font,cxTextAttr attr);

void cxTextureFactoryDelFile(cxConstChars file);

CX_C_END

#endif
