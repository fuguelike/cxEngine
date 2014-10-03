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
#include "cxAnimate.h"
CX_SETTER_DEF(cxAnimateItem, id)
{
    cxString id = cxJsonToString(value);
    CX_RETAIN_SET(this->id, id);
}
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
CX_SETTER_DEF(cxAnimateItem, flipx)
{
    this->flipx = cxJsonToBool(value, this->flipx);
}
CX_SETTER_DEF(cxAnimateItem, flipy)
{
    this->flipy = cxJsonToBool(value, this->flipy);
}

CX_OBJECT_TYPE(cxAnimateItem, cxAction)
{
    CX_PROPERTY_SETTER(cxAnimateItem, id);
    CX_PROPERTY_SETTER(cxAnimateItem, key);
    CX_PROPERTY_SETTER(cxAnimateItem, texture);
    CX_PROPERTY_SETTER(cxAnimateItem, time);
    CX_PROPERTY_SETTER(cxAnimateItem, flipx);
    CX_PROPERTY_SETTER(cxAnimateItem, flipy);
}
CX_OBJECT_INIT(cxAnimateItem, cxObject)
{
    this->time = 0;
    this->value = 0;
    this->flipx = false;
    this->flipy = false;
}
CX_OBJECT_FREE(cxAnimateItem, cxObject)
{
    CX_RELEASE(this->id);
    CX_RELEASE(this->key);
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxAnimateItem, cxObject)

static cxAnimateItem cxAnimateItemCopy(cxAnimateItem this)
{
    cxAnimateItem cp = CX_CREATE(cxAnimateItem);
    CX_RETAIN_SET(cp->id, this->id);
    CX_RETAIN_SET(cp->key, this->key);
    CX_RETAIN_SET(cp->texture, this->texture);
    cp->time = this->time;
    return cp;
}

static cxHashKey cxAnimateItemKey(cxAnimateItem this)
{
    CX_ASSERT(this->key != NULL, "must set cxAnimateItem key");
    cxConstChars key = this->id != NULL ? cxStringBody(this->id) : cxStringBody(this->key);
    return cxHashStrKey(key);
}

static cxAnimateItem cxAnimateItemGet(cxAnimate this,cxArray items, cxAny any, cxInt index)
{
    cxObject item = any;
    if(item->cxType == cxAnimateItemTypeName){
        return any;
    }
    if(item->cxType != cxStringTypeName){
        return NULL;
    }
    cxConstChars key = cxStringBody(any);
    cxAnimateItem frame = cxHashGet(this->frames, cxHashStrKey(key));
    if(frame == NULL){
        return NULL;
    }
    //update frame at array index
    frame = cxAnimateItemCopy(frame);
    cxArrayUpdate(items, frame, index);
    return frame;
}

static void cxAnimateInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAnimate);
    CX_ASSERT_TYPE(cxActionGetView(this), cxSprite);
    this->index = 0;
    cxFloat time = cxActionGetTime(this);
    cxArray items = cxAnimateGetGroup(this,this->name);
    CX_ASSERT(items != NULL, "group name not found");
    cxFloat dt = this->time / (cxFloat)cxArrayLength(items);
    cxFloat value = 0;
    cxInt i = 0;
    CX_ARRAY_FOREACH(items, e){
        cxAnimateItem item = cxAnimateItemGet(this,items,cxArrayObject(e),i);
        CX_ASSERT_TYPE(item, cxAnimateItem);
        time += item->time;
        value += dt  + item->time;
        item->value = value;
        i++;
    }
    cxActionSetTime(this, time);
}

static void cxAnimateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxAnimate);
    CX_ASSERT_VALUE(cxActionGetView(this), cxSprite, view);
    cxArray items = cxAnimateGetGroup(this,this->name);
    CX_ASSERT(items != NULL, "group name not found ");
    cxInt i = 0;
    CX_RETURN(this->index >= cxArrayLength(items));
    CX_ARRAY_FOREACH(items, e){
        i++;
        cxAnimateItem item = cxArrayObject(e);
        CX_ASSERT_TYPE(item, cxAnimateItem);
        if(cxActionGetTimeElapsed(this) > item->value){
            continue;
        }
        if(this->index == i){
            break;
        }
        if(item->texture != NULL){
            cxSpriteSetTexture(view, item->texture);
        }
        if(item->texture != NULL && item->key != NULL){
            cxSpriteSetTextureKey(view, cxStringBody(item->key));
        }
        cxSpriteSetFlipX(view, item->flipx);
        cxSpriteSetFlipY(view, item->flipy);
        this->index = i;
        CX_EVENT_FIRE(this, onFrame);
        break;
    }
}

static void cxAnimateReset(cxAny pav)
{
    cxAnimate this = pav;
    this->index = 0;
    cxActionSetTime(this, this->time);
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
    cxActionSetTime(this, this->time);
}
CX_SETTER_DEF(cxAnimate, frames)
{
    cxJson frames = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(frames, item)
    {
        cxAnimateItem frame = cxObjectCreateUseJson(item);
        CX_ASSERT_TYPE(frame, cxAnimateItem);
        cxHashKey key = cxAnimateItemKey(frame);
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
        cxArray items = cxAnimateGetGroup(this,key);
        cxJson frames = cxJsonToArray(item);
        CX_JSON_ARRAY_EACH_BEG(frames, ats)
        {
            if(cxJsonIsObject(ats)){
                cxAnimateItem frame = cxObjectCreateUseJson(ats);
                CX_ASSERT_TYPE(frame, cxAnimateItem);
                cxArrayAppend(items, frame);
                continue;
            }
            if(cxJsonIsString(ats)){
                cxConstChars ik = cxJsonToConstChars(ats);
                cxString frameKey = cxStringAllocChars(ik);
                cxArrayAppend(items, frameKey);
                CX_RELEASE(frameKey);
                continue;
            }
        }
        CX_JSON_ARRAY_EACH_END(frames, ats)
        CX_RELEASE(key);
    }
    CX_JSON_OBJECT_EACH_END(groups, item)
}
CX_SETTER_DEF(cxAnimate, name)
{
    cxConstChars name = cxJsonToConstChars(value);
    if(name != NULL){
        cxAnimateRunGroup(this, name);
    }
}

cxArray cxAnimateGetGroup(cxAny pav,cxString name)
{
    CX_ASSERT_THIS(pav, cxAnimate);
    CX_ASSERT(name != NULL, "must set group name");
    CX_RETURN(name == NULL, NULL);
    cxConstChars cname = cxStringBody(name);
    cxArray items = cxHashGet(this->groups, cxHashStrKey(cname));
    if(items == NULL){
        items = CX_ALLOC(cxArray);
        cxHashSet(this->groups, cxHashStrKey(cname), items);
        CX_RELEASE(items);
    }
    return items;
}

CX_OBJECT_TYPE(cxAnimate, cxAction)
{
    CX_PROPERTY_SETTER(cxAnimate, forever);
    CX_PROPERTY_SETTER(cxAnimate, time);
    CX_PROPERTY_SETTER(cxAnimate, name);
    CX_PROPERTY_SETTER(cxAnimate, frames);
    CX_PROPERTY_SETTER(cxAnimate, groups);
}
CX_OBJECT_INIT(cxAnimate, cxAction)
{
    this->forever = true;
    CX_SET(cxAction, this, Init, cxAnimateInit);
    CX_SET(cxAction, this, Step, cxAnimateStep);
    CX_SET(cxAction, this, Exit, cxAnimateExit);
    CX_SET(cxAction, this, Reset, cxAnimateReset);
    this->groups = CX_ALLOC(cxHash);
    this->frames = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxAnimate, cxAction)
{
    CX_RELEASE(this->frames);
    CX_RELEASE(this->groups);
    CX_RELEASE(this->name);
    CX_EVENT_RELEASE(this->onFrame);
}
CX_OBJECT_TERM(cxAnimate, cxAction)

void cxAnimateRunGroup(cxAny pav,cxConstChars name)
{
    CX_ASSERT_THIS(pav, cxAnimate);
    cxActionReset(this);
    CX_RETURN(name == NULL);
    cxString str = cxStringConstChars(name);
    CX_RETAIN_SWAP(this->name, str);
}











