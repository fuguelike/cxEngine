//
//  cxTextureCache.h
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTextureCache_h
#define cxEngine_cxTextureCache_h

#include "cxTexturePVR.h"
#include "cxTexturePNG.h"
#include "cxTextureJSON.h"
#include "cxTextureTXT.h"
#include "cxTexturePKM.h"
#include "cxTextureJPG.h"

CX_C_BEGIN

CX_DEF(cxTextureCache, cxObject)
    cxHash caches;
CX_END(cxTextureCache, cxObject)

cxTexture cxTextureCacheLoadFile(cxConstChars file);

cxTexture cxTextureCreateText(cxString txt,cxString font,cxTextAttr attr);

void cxTextureCacheUnloadFile(cxConstChars file);

void cxTextureCacheClear();

cxTexture cxTextureCreate(cxConstChars file);

void cxTextureCacheClearGroup(cxConstChars group);

CX_C_END

#endif
