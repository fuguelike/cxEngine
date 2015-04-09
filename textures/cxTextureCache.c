//
//  cxTextureCache.c
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxMemStream.h>
#include "cxTextureCache.h"

#define CX_DEFAULT_GROUP "__cxTexturesGroupName__"

static cxBool cxTextureCacheMemory(cxAny pthis,cxAny engine)
{
    CX_ASSERT_THIS(pthis, cxTextureCache);
    cxTextureCacheClear(this);
    return false;
}

CX_METHOD_DEF(cxTextureCache, URLFilter, cxConstChars,cxConstChars ourl)
{
    return cxStrBody(cxStrConstChars(ourl));
}
CX_TYPE(cxTextureCache, cxObject)
{
    CX_METHOD(cxTextureCache, URLFilter);
}
CX_INIT(cxTextureCache, cxObject)
{
    this->caches = CX_ALLOC(cxHash);
    cxHash group = CX_CREATE(cxHash);
    cxHashSet(this->caches, cxHashStrKey(CX_DEFAULT_GROUP), group);
    cxMessageAppend(this, cxTextureCacheMemory, cxEngineNoticMemory);
}
CX_FREE(cxTextureCache, cxObject)
{
    CX_RELEASE(this->caches);
}
CX_TERM(cxTextureCache, cxObject)

//@GroupName:candy.xml?green.png
static cxHash cxTextureCacheGroup(cxConstChars file,cxChars files)
{
    CX_ASSERT(file != NULL && files != NULL, "args error");
    cxChar group[PATH_MAX] = {0};
    cxTextureCache this = cxTextureCacheInstance();
    file = CX_CALL(this, URLFilter, CX_M(cxConstChars,cxConstChars),file);
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

void cxTextureCacheClear(cxTextureCache this)
{
    CX_HASH_FOREACH(this->caches, ele, tmp){
        cxHashClear(ele->any);
    }
}

void cxTextureCacheClearGroup(cxConstChars group)
{
    cxTextureCache this = cxTextureCacheInstance();
    cxHash groups = cxHashGet(this->caches, cxHashStrKey(group));
    if(groups != NULL){
        cxHashClear(groups);
    }
}

void cxTextureCacheUnloadFileWithKey(cxConstChars file,cxConstChars key)
{
    cxChar files[PATH_MAX]={0};
    cxHash groups = cxTextureCacheGroup(file,files);
    cxHashDel(groups, cxHashStrKey(key));
}

void cxTextureCacheUnloadFile(cxConstChars file)
{
    cxChar files[PATH_MAX]={0};
    cxHash groups = cxTextureCacheGroup(file,files);
    cxHashDel(groups, cxHashStrKey(files));
}

cxTexture cxTextureCreateText(cxStr txt,cxStr font,cxTextAttr attr)
{
    cxTextureTXT texture = CX_CREATE(cxTextureTXT);
    texture->attr = attr;
    CX_RETAIN_SWAP(texture->string, txt);
    CX_RETAIN_SWAP(texture->font, font);
    CX_CALL(texture, Load, CX_M(cxBool,cxStream),NULL);
    return (cxTexture)texture;
}

cxTexture cxTextureCreateWithEXT(cxConstChars file,cxConstChars ext)
{
    cxTexture texture = NULL;
    CX_ASSERT(file != NULL && ext != NULL, "file args error");
    cxStream stream = cxAssetsStreamCreate(file);
    if(stream == NULL){
        CX_ERROR("create stream from file %s failed",file);
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
    }else if(cxConstCharsEqu(ext, ".xtf")){
        texture = cxTextureXTFLoadStream(stream);
    }else if(cxConstCharsEqu(ext, ".mtf")){
        texture = cxTextureMTFCreate(file);
    }else{
        CX_ERROR("load texture failed %s,not support ext %s",file,ext);
    }
    return texture;
}

cxTexture cxTextureCreate(cxConstChars file)
{
    CX_ASSERT(file != NULL, "file args error");
    char *ext = strrchr(file, '.');
    return cxTextureCreateWithEXT(file, ext);
}

cxTexture cxTextureCacheLoadFileWithKey(cxConstChars file,cxConstChars key)
{
    cxChar path[PATH_MAX]={0};
    cxHash groups = cxTextureCacheGroup(file,path);
    cxHashKey hkey = cxHashStrKey(key);
    cxTexture texture = cxHashGet(groups, hkey);
    if(texture != NULL){
        return texture;
    }
    if((texture = cxTextureCreate(path)) != NULL){
        cxHashSet(groups, hkey, texture);
    }
    return texture;
}
//use global cached
cxTexture cxTextureCacheLoadFile(cxConstChars format,...)
{
    cxChar file[PATH_MAX]={0};
    va_list ap;
    va_start(ap, format);
    vsnprintf(file, PATH_MAX, format, ap);
    va_end(ap);
    
    cxChar path[PATH_MAX]={0};
    cxHash groups = cxTextureCacheGroup(file,path);
    cxHashKey hkey = cxHashStrKey(path);
    
    cxTexture texture = cxHashGet(groups, hkey);
    if(texture != NULL){
        return texture;
    }
    if((texture = cxTextureCreate(path)) != NULL){
        cxHashSet(groups, hkey, texture);
    }
    return texture;
}


