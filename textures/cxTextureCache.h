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
#include "cxTextureXTF.h"
#include "cxTextureMTF.h"

CX_C_BEGIN

CX_DEF(cxTextureCache, cxObject)
    cxHash caches;
CX_END(cxTextureCache, cxObject)

//load with cache key
cxTexture cxTextureCacheLoadFileWithKey(cxConstChars file,cxConstChars key);

cxTexture cxTextureCacheLoadFile(cxConstChars format,...);

cxTexture cxTextureCreateText(cxStr txt,cxStr font,cxTextAttr attr);

void cxTextureCacheUnloadFileWithKey(cxConstChars file,cxConstChars key);

void cxTextureCacheUnloadFile(cxConstChars file);

void cxTextureCacheClear(cxTextureCache this);

cxTexture cxTextureCreate(cxConstChars file);

cxTexture cxTextureCreateWithEXT(cxConstChars file,cxConstChars ext);

void cxTextureCacheClearGroup(cxConstChars group);

CX_C_END

#endif
