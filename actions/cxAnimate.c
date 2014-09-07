//
//  cxAnimate.c
//  cxEngine
//
//  Created by xuhua on 11/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <views/cxSprite.h>
#include <textures/cxTextureFactory.h>
#include <engine/cxTexture.h>
#include <engine/cxUtil.h>
#include <engine/cxUrlPath.h>
#include "cxAnimate.h"

CX_SETTER_DEF(cxAnimateItem, key)
{
    cxString key = cxJsonToString(value);
    CX_RETAIN_SET(this->key, key);
}
CX_SETTER_DEF(cxAnimateItem, texture)
{
    cxConstChars file = cxJsonToConstChars(value);
    CX_ASSERT(file != NULL, "cxAnimateItem must set texture");
    cxAny texture = cxTextureFactoryLoadFile(file);
    CX_RETURN(texture == NULL);
    CX_RETAIN_SET(this->texture, texture);
}
CX_SETTER_DEF(cxAnimateItem, time)
{
    this->time = cxJsonToDouble(value, this->time);
}

CX_OBJECT_TYPE(cxAnimateItem, cxAction)
{
    CX_PROPERTY_SETTER(cxAnimateItem, key);
    CX_PROPERTY_SETTER(cxAnimateItem, texture);
    CX_PROPERTY_SETTER(cxAnimateItem, time);
}
CX_OBJECT_INIT(cxAnimateItem, cxObject)
{
    this->time = 0;
}
CX_OBJECT_FREE(cxAnimateItem, cxObject)
{
    CX_RELEASE(this->key);
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxAnimateItem, cxObject)

static cxAnimateItem cxAnimateItemGet(cxAny pav,cxAny any)
{
    cxAnimate this = pav;
    cxObject item = any;
    if(item->cxType == cxAnimateItemTypeName){
        return any;
    }
    if(item->cxType == cxStringTypeName){
        cxConstChars key = cxStringBody(any);
        return cxHashGet(this->frames, cxHashStrKey(key));
    }
    return NULL;
}

static void cxAnimateInit(cxAny pav)
{
    cxAnimate this = pav;
    CX_ASSERT(CX_INSTANCE_OF(this->cxAction.view, cxSprite), "cxAnimate action view must is cxSprite");
    this->index = 0;
    this->cxAction.time = this->time;
    cxHash items = cxAnimateGetGroup(this,this->key);
    cxFloat dt = this->time / (cxFloat)cxHashLength(items);
    cxFloat i = 0;
    CX_HASH_FOREACH(items, ele, tmp){
        cxAnimateItem item = cxAnimateItemGet(this,ele->any);
        CX_ASSERT(item != NULL, "get item error");
        this->cxAction.time += item->time;
        i += dt  + item->time;
        item->value = i;
    }
}

static void cxAnimateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxAnimate this = pav;
    cxHash items = cxAnimateGetGroup(this,this->key);
    cxInt i = 0;
    CX_RETURN(this->index >= cxHashLength(items));
    CX_HASH_FOREACH(items, ele, tmp){
        i++;
        cxAnimateItem item = cxAnimateItemGet(this,ele->any);
        CX_ASSERT(item != NULL, "get item error");
        if(this->cxAction.timeElapsed > item->value){
            continue;
        }
        if(this->index == i){
            break;
        }
        if(item->texture != NULL){
            cxSpriteSetTexture(this->cxAction.view, item->texture);
        }
        if(item->texture != NULL && item->key != NULL){
            cxSpriteSetTextureKey(this->cxAction.view, cxStringBody(item->key));
        }
        this->index = i;
        CX_EVENT_FIRE(this, onFrame);
        break;
    }
}

static void cxAnimateReset(cxAny pav)
{
    cxAnimate this = pav;
    this->index = 0;
    this->cxAction.time = this->time;
}

static cxBool cxAnimateExit(cxAny pav)
{
    cxAnimate this = pav;
    if(this->forever){
        cxActionReset(pav);
        return false;
    }
    return true;
}

CX_SETTER_DEF(cxAnimate, time)
{
    this->time = cxJsonToDouble(value, this->time);
    this->cxAction.time = this->time;
}
CX_SETTER_DEF(cxAnimate, frames)
{
    cxJson frames = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(frames, item)
    {
        //first id
        cxConstChars id = cxJsonConstChars(item, "id");
        cxConstChars ik = cxJsonConstChars(item, "key");
        CX_ASSERT(ik != NULL, "key must set");
        cxHashKey key = id != NULL ? cxHashStrKey(id) : cxHashStrKey(ik);
        cxAny frame = cxObjectCreateWithJson(item);
        CX_ASSERT(CX_INSTANCE_OF(frame, cxAnimateItem), "type error");
        cxHashSet(this->frames, key, frame);
    }
    CX_JSON_ARRAY_EACH_END(frames, item)
}
CX_SETTER_DEF(cxAnimate, forever)
{
    this->forever = cxJsonToBool(value, this->forever);
}
CX_SETTER_DEF(cxAnimate, groups)
{
    cxJson groups = value;
    CX_JSON_OBJECT_EACH_BEG(groups, item)
    {
        CX_ASSERT(cxJsonIsArray(item), "must is array");
        cxString key = cxStringAllocChars(itemKey);
        cxHash items = cxAnimateGetGroup(this,key);
        cxJson frames = cxJsonToArray(item);
        CX_JSON_ARRAY_EACH_BEG(frames, ats)
        {
            if(cxJsonIsString(ats)){
                cxConstChars ik = cxJsonToConstChars(ats);
                cxString im = cxStringAllocChars(ik);
                cxHashSet(items, cxHashStrKey(ik), im);
                CX_RELEASE(im);
            }else if(cxJsonIsObject(ats)){
                cxConstChars ik = cxJsonConstChars(ats, "key");
                CX_ASSERT(ik != NULL, "key must set");
                cxAny frame = cxObjectCreateWithJson(ats);
                CX_ASSERT(CX_INSTANCE_OF(frame, cxAnimateItem), "type error");
                cxHashSet(items, cxHashStrKey(ik), frame);
            }
        }
        CX_JSON_ARRAY_EACH_END(frames, ats)
        CX_RELEASE(key);
    }
    CX_JSON_OBJECT_EACH_END(groups, item)
}
CX_SETTER_DEF(cxAnimate, key)
{
    cxConstChars key = cxJsonToConstChars(value);
    if(key != NULL){
        cxAnimateSetKey(this, key);
    }
}

//from default group get item
cxAnimateItem cxAnimateGetFrame(cxAny pav,cxConstChars key)
{
    cxAnimate this = pav;
    return cxHashGet(this->frames, cxHashStrKey(key));
}

cxHash cxAnimateGetGroup(cxAny pav,cxString key)
{
    cxAnimate this = pav;
    CX_RETURN(key == NULL, this->frames);
    cxConstChars name = cxStringBody(key);
    cxHash items = cxHashGet(this->groups, cxHashStrKey(name));
    if(items == NULL){
        items = CX_ALLOC(cxHash);
        cxHashSet(this->groups, cxHashStrKey(name), items);
        CX_RELEASE(items);
    }
    return items;
}

CX_OBJECT_TYPE(cxAnimate, cxAction)
{
    CX_PROPERTY_SETTER(cxAnimate, forever);
    CX_PROPERTY_SETTER(cxAnimate, time);
    CX_PROPERTY_SETTER(cxAnimate, key);
    CX_PROPERTY_SETTER(cxAnimate, frames);
    CX_PROPERTY_SETTER(cxAnimate, groups);
}
CX_OBJECT_INIT(cxAnimate, cxAction)
{
    this->forever = true;
    CX_METHOD_SET(this->cxAction.Exit, cxAnimateExit);
    CX_METHOD_SET(this->cxAction.Init, cxAnimateInit);
    CX_METHOD_SET(this->cxAction.Step, cxAnimateStep);
    CX_METHOD_SET(this->cxAction.Reset, cxAnimateReset);
    this->groups = CX_ALLOC(cxHash);
    this->frames = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxAnimate, cxAction)
{
    CX_RELEASE(this->frames);
    CX_RELEASE(this->groups);
    CX_RELEASE(this->key);
    CX_EVENT_RELEASE(this->onFrame);
}
CX_OBJECT_TERM(cxAnimate, cxAction)

void cxAnimateSetKey(cxAny pav,cxConstChars name)
{
    cxAnimate this = pav;
    cxActionReset(this);
    CX_RETURN(name == NULL);
    cxString str = cxStringConstChars(name);
    CX_RETAIN_SWAP(this->key, str);
}

cxAnimate cxAnimateCreate(cxFloat time,cxConstChars key)
{
    cxAnimate this = CX_CREATE(cxAnimate);
    this->time = time;
    cxAnimateSetKey(this, key);
    return this;
}










