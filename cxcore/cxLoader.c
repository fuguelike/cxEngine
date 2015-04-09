//
//  cxLoader.c
//  cxCore
//
//  Created by xuhua on 9/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxJson.h"
#include "cxLoader.h"
#include "cxStack.h"

static cxStack loaderStack = NULL;

void cxLoaderClear()
{
    CX_ASSERT(loaderStack != NULL, "loaderStack not init");
    cxStackClear(loaderStack);
}

void cxLoaderInit()
{
    loaderStack = CX_ALLOC(cxStack);
}

void cxLoaderFree()
{
    CX_RELEASE(loaderStack);
    loaderStack = NULL;
}

void cxLoaderPush(cxAny object)
{
    cxStackPush(loaderStack, object);
}

void cxLoaderPop()
{
    cxStackPop(loaderStack);
}

cxAny cxLoaderTop()
{
    return cxStackTop(loaderStack);
}

CX_METHOD_DEF(cxLoader, SetItem, void,cxConstChars key, cxAny object)
{
    CX_ASSERT(key != NULL && object != NULL, "args error");
    cxHashSet(this->objects, cxHashStrKey(key), object);
}
CX_METHOD_DEF(cxLoader, GetItem, cxAny, cxConstChars key)
{
    CX_ASSERT(key != NULL, "args error");
    return cxHashGet(this->objects, cxHashStrKey(key));
}
CX_TYPE(cxLoader, cxObject)
{
    CX_METHOD(cxLoader, SetItem);
    CX_METHOD(cxLoader, GetItem);
}
CX_INIT(cxLoader, cxObject)
{
    this->objects = CX_ALLOC(cxHash);
}
CX_FREE(cxLoader, cxObject)
{
    CX_RELEASE(this->Root);
    CX_RELEASE(this->objects);
}
CX_TERM(cxLoader, cxObject)

cxAny cxLoaderGet(cxAny loader,cxConstChars id)
{
    CX_ASSERT_THIS(loader, cxLoader);
    return cxHashGet(this->objects, cxHashStrKey(id));
}

static void cxLoaderRun(cxLoader this,cxJson json)
{
    cxLoaderPush(this);
    //create main root
    cxAny root = NULL;
    cxJson entry = cxJsonObject(json, "cxMain");
    if(entry != NULL){
        root = cxJsonMakeObject(entry);
    }else{
        root = cxJsonMakeObject(json);
    }
    CX_RETAIN_SWAP(this->Root, root);
    cxLoaderPop();
}

cxAny cxLoaderFire(cxAny loader,cxConstChars path)
{
    CX_ASSERT_THIS(loader, cxLoader);
    cxJson json = cxJsonRead(path);
    CX_ASSERT(json != NULL, "json file %s read error",path);
    cxLoaderRun(this, json);
    return this->Root;
}

cxLoader cxLoaderJson(cxJson json)
{
    cxLoader this = CX_CREATE(cxLoader);
    CX_ASSERT(json != NULL, "json error");
    cxLoaderRun(this, json);
    return this;
}

cxAny cxLoaderRoot(cxConstChars path)
{
    cxLoader this = cxLoaderCreate(path);
    CX_ASSERT(this->Root != NULL, "root error");
    return this->Root;
}

cxLoader cxLoaderCreate(cxConstChars path)
{
    cxLoader this = CX_CREATE(cxLoader);
    cxLoaderFire(this, path);
    return this;
}