//
//  cxTextureFactory.c
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxMemStream.h>
#include "cxTextureFactory.h"

#define CX_DEFAULT_GROUP "__default__"

static cxTextureFactory instance = NULL;

cxTextureFactory cxTextureFactoryInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxTextureFactory);
    }
    return instance;
}

static void cxTextureFactoryDestroy(cxEvent *event)
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
    
    cxHash group = CX_CREATE(cxHash);
    cxHashSet(this->caches, cxHashStrKey(CX_DEFAULT_GROUP), group);
    
    CX_SLOT_CONNECT(engine->onMemory, this, onMemory, cxTextureFactoryMemory);
    CX_EVENT_QUICK(engine->onFree, cxTextureFactoryDestroy);
}
CX_OBJECT_FREE(cxTextureFactory, cxObject)
{
    CX_SLOT_RELEASE(this->onMemory);
    CX_RELEASE(this->caches);
}
CX_OBJECT_TERM(cxTextureFactory, cxObject)

//group:candy.xml?green.png
static cxHash cxTextureFactoryGroup(cxConstChars file,cxChar *files)
{
    CX_ASSERT(file != NULL && files != NULL, "args error");
    cxChar group[PATH_MAX] = {0};
    cxTextureFactory this = cxTextureFactoryInstance();
    cxInt len = strlen(file);
    cxConstChars gs = strrchr(file, ':');
    if(gs != NULL){
        int glen = strlen(gs);
        memcpy(group, file, len - glen);
        memcpy(files, gs + 1, glen - 1);
    }else{
        memcpy(group, CX_DEFAULT_GROUP, strlen(CX_DEFAULT_GROUP));
        memcpy(files, file, len);
    }
    cxHash groups = cxHashGet(this->caches, cxHashStrKey(group));
    if(groups == NULL){
        groups = CX_CREATE(cxHash);
        cxHashSet(this->caches, cxHashStrKey(group), groups);
    }
    return groups;
}

void cxTextureFactoryCleanGroup(cxConstChars group)
{
    cxTextureFactory this = cxTextureFactoryInstance();
    cxHash groups = cxHashGet(this->caches, cxHashStrKey(group));
    if(groups != NULL){
        cxHashClean(groups);
    }
}

void cxTextureFactoryUnloadFile(cxConstChars file)
{
    cxChar files[PATH_MAX]={0};
    cxHash groups = cxTextureFactoryGroup(file,files);
    cxHashDel(groups, cxHashStrKey(files));
}

cxTexture cxTextureFactoryLoadText(const cxString txt,const cxString font,cxTextAttr attr)
{
    cxTextureTXT texture = CX_CREATE(cxTextureTXT);
    texture->attr = attr;
    CX_RETAIN_SWAP(texture->string, txt);
    CX_RETAIN_SWAP(texture->fontfile, font);
    cxTextureLoad((cxTexture)texture,NULL);
    return (cxTexture)texture;
}

static cxTexture cxTextureCreate(cxConstChars file)
{
    cxTexture texture = NULL;
    CX_ASSERT(file != NULL, "file args error");
    cxStream stream = cxAssetsStreamCreate(file);
    if(stream == NULL){
        CX_ERROR("create stream from file %s failed",file);
        return NULL;
    }
    char *ext = strrchr(file, '.');
    if(ext == NULL){
        CX_ERROR("unknow file ext name");
        return NULL;
    }
    if(cxConstCharsEqu(ext, ".png")){
        texture = cxTexturePNGLoadStream(stream);
    }else if(cxConstCharsEqu(ext, ".pvr")){
        texture = cxTexturePVRLoadStream(stream);
    }else if(cxConstCharsEqu(ext, ".xml")){
        texture = cxTextureXMLLoadStream(stream);
    }else if(cxConstCharsEqu(ext, ".pkm")){
        texture = cxTexturePKMLoadStream(stream);
    }else if(cxConstCharsEqu(ext, ".jpg") || cxConstCharsEqu(ext, ".jpeg")){
        texture = cxTextureJPGLoadStream(stream);
    }else{
        CX_ERROR("load texture failed %s,not support ext %s",file,ext);
    }
    return texture;
}

//use global cached
cxTexture cxTextureFactoryLoadFile(cxConstChars file)
{
    cxChar files[PATH_MAX]={0};
    cxHash groups = cxTextureFactoryGroup(file,files);
    cxTexture texture = cxHashGet(groups, cxHashStrKey(files));
    if(texture != NULL){
        return texture;
    }
    if((texture = cxTextureCreate(files)) != NULL){
        cxHashSet(groups, cxHashStrKey(files), texture);
    }
    return texture;
}


