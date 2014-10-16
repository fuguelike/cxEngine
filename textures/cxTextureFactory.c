//
//  cxTextureFactory.c
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <engine/cxRegex.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxMemStream.h>
#include "cxTextureFactory.h"

#define CX_DEFAULT_GROUP "__cxTexturesGroupName__"

static void cxTextureFactoryMemory(cxTextureFactory this)
{
    cxHashClear(this->caches);
}

CX_OBJECT_TYPE(cxTextureFactory, cxObject)
{
    
}
CX_OBJECT_INIT(cxTextureFactory, cxObject)
{
    cxEngine engine = cxEngineInstance();
    this->caches = CX_ALLOC(cxHash);
    
    cxHash group = CX_CREATE(cxHash);
    cxHashSet(this->caches, cxHashStrKey(CX_DEFAULT_GROUP), group);
    CX_CON(cxEngine, engine, onMemory, this, cxTextureFactoryMemory);
}
CX_OBJECT_FREE(cxTextureFactory, cxObject)
{
    CX_SLOT_RELEASE(this->onMemory);
    CX_RELEASE(this->caches);
}
CX_OBJECT_TERM(cxTextureFactory, cxObject)

//@GroupName:candy.xml?green.png
static cxHash cxTextureFactoryGroup(cxConstChars file,cxChars files)
{
    CX_ASSERT(file != NULL && files != NULL, "args error");
    cxChar group[PATH_MAX] = {0};
    cxTextureFactory this = cxTextureFactoryInstance();
    cxInt len = (cxInt)strlen(file);
    if(file[0] == '@'){
        cxConstChars gs = strrchr(file, ':');
        CX_ASSERT(gs != NULL, "@ prefix must has : char");
        int glen = (cxInt)strlen(gs);
        memcpy(group, file + 1, len - glen - 1);
        memcpy(files, gs + 1, glen - 1);
    }else{
        memcpy(group, CX_DEFAULT_GROUP, strlen(CX_DEFAULT_GROUP));
        memcpy(files, file, len);
    }
    cxHash groups = cxHashGet(this->caches, cxHashStrKey(group));
    if(groups == NULL){
        groups = CX_ALLOC(cxHash);
        cxHashSet(this->caches, cxHashStrKey(group), groups);
        CX_RELEASE(groups);
    }
    return groups;
}

void cxTextureFactoryClear()
{
    cxTextureFactory this = cxTextureFactoryInstance();
    CX_HASH_FOREACH(this->caches, ele, tmp){
        cxHashClear(ele->any);
    }
}

void cxTextureFactoryClearGroup(cxConstChars group)
{
    cxTextureFactory this = cxTextureFactoryInstance();
    cxHash groups = cxHashGet(this->caches, cxHashStrKey(group));
    if(groups != NULL){
        cxHashClear(groups);
    }
}

void cxTextureFactoryUnloadFile(cxConstChars file)
{
    cxChar files[PATH_MAX]={0};
    cxHash groups = cxTextureFactoryGroup(file,files);
    cxHashDel(groups, cxHashStrKey(files));
}

cxTexture cxTextureFactoryLoadText(cxString txt,cxString font,cxTextAttr attr)
{
    cxTextureTXT texture = CX_CREATE(cxTextureTXT);
    texture->attr = attr;
    CX_RETAIN_SWAP(texture->string, txt);
    CX_RETAIN_SWAP(texture->font, font);
    cxTextureLoad((cxTexture)texture,NULL);
    return (cxTexture)texture;
}

cxTexture cxTextureCreate(cxConstChars file)
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
    }else if(cxConstCharsEqu(ext, ".json")){
        texture = cxTextureJSONLoadStream(stream);
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


