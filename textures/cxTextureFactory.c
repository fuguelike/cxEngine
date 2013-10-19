//
//  cxTextureFactory.c
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <streams/cxAssetsStream.h>
#include "cxTextureFactory.h"

static cxTextureFactory instance = NULL;

cxTextureFactory cxTextureFactoryInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxTextureFactory);
    }
    return instance;
}

static void cxTextureFactoryDestroy(cxAny engine,cxAny arg)
{
    CX_RELEASE(instance);
    instance = NULL;
}

static void cxTextureFactoryMemory(cxAny factory)
{
    cxTextureFactory this = factory;
    cxHashClean(this->caches);
}

CX_OBJECT_INIT(cxTextureFactory, cxObject)
{
    cxEngine engine = cxEngineInstance();
    this->caches = CX_ALLOC(cxHash);
    
    CX_SLOT_QUICK(engine->onMemory, this, onMemory, cxTextureFactoryMemory);
    CX_EVENT_QUICK(engine->onFree, cxTextureFactoryDestroy);
}
CX_OBJECT_FREE(cxTextureFactory, cxObject)
{
    CX_SLOT_RELEASE(this->onMemory);
    CX_RELEASE(this->caches);
}
CX_OBJECT_TERM(cxTextureFactory, cxObject)

void cxTextureDelFile(cxConstChars file)
{
    cxTextureFactory factory = cxTextureFactoryInstance();
    cxHashDel(factory->caches, cxHashStrKey(file));
}

cxTexture cxTextureLoadFile(cxConstChars file)
{
    cxTextureFactory factory = cxTextureFactoryInstance();
    cxTexture texture = cxHashGet(factory->caches, cxHashStrKey(file));
    if(texture != NULL){
        return texture;
    }
    cxStream stream = cxAssetsStreamCreate(file);
    if(stream == NULL){
        CX_ERROR("create stream from file %s failed",file);
        return NULL;
    }
    char *ext = strrchr(file, '.');
    if(strcasecmp(ext, ".png") == 0){
        texture = cxTexturePNGLoadStream(stream);
    }else if(strcasecmp(ext, ".pvr") == 0){
        texture = cxTexturePVRLoadStream(stream);
    }else if(strcasecmp(ext, ".xml") == 0){
        texture = cxTextureXMLLoadStream(stream);
    }else{
        CX_ERROR("load texture failed %s",file);
    }
    if(texture != NULL) {
        cxHashSet(factory->caches, cxHashStrKey(file), texture);
    }
    return texture;
}


