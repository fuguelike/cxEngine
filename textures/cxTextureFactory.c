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

static cxTextureFactory instance = NULL;

const luaL_Reg cxTextureFactoryInstanceMethods[] = {
    CX_LUA_SUPER(cxObject)
};

static cxInt cxTextureFactoryLuaCreate(lua_State *L)
{
    cxConstChars file = luaL_checkstring(L, 1);
    cxBool cache = cxLuaBoolValue(L, 2, true);
    cxTexture texture = cache ? cxTextureFactoryLoadFile(file) : cxTextureCreate(file);
    CX_LUA_PUSH_OBJECT(texture);
    return 1;
}

const luaL_Reg cxTextureFactoryTypeMethods[] = {
    {"create",cxTextureFactoryLuaCreate},
    {NULL,NULL},
};

void cxTextureFactoryTypeInit()
{
    CX_LUA_LOAD_TYPE(cxTextureFactory);
}

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
    
    CX_SLOT_CONNECT(engine->onMemory, this, onMemory, cxTextureFactoryMemory);
    CX_EVENT_QUICK(engine->onFree, cxTextureFactoryDestroy);
}
CX_OBJECT_FREE(cxTextureFactory, cxObject)
{
    CX_SLOT_RELEASE(this->onMemory);
    CX_RELEASE(this->caches);
}
CX_OBJECT_TERM(cxTextureFactory, cxObject)

void cxTextureFactoryUnloadFile(cxConstChars file)
{
    cxTextureFactory factory = cxTextureFactoryInstance();
    cxHashDel(factory->caches, cxHashStrKey(file));
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
    }else if(cxConstCharsEqu(ext, ".xml")){
        texture = cxTextureXMLLoadStream(stream);
    }else if(cxConstCharsEqu(ext, ".pkm")){
        texture = cxTexturePKMLoadStream(stream);
    }else if(cxConstCharsEqu(ext, ".jpg") || cxConstCharsEqu(ext, ".jpeg")){
        texture = cxTextureJPGLoadStream(stream);
    }else{
        CX_ERROR("load texture failed %s",file);
    }
    return texture;
}

//use global cached
cxTexture cxTextureFactoryLoadFile(cxConstChars file)
{
    cxTextureFactory factory = cxTextureFactoryInstance();
    cxTexture texture = cxHashGet(factory->caches, cxHashStrKey(file));
    if(texture != NULL){
        return texture;
    }
    if((texture = cxTextureCreate(file)) != NULL){
        cxHashSet(factory->caches, cxHashStrKey(file), texture);
    }
    return texture;
}


