//
//  cxAnimate.c
//  cxEngine
//
//  Created by xuhua on 11/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <views/cxSprite.h>
#include <textures/cxTextureCache.h>
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
    cxAny texture = cxTextureCacheLoadFile(file);
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

CX_TYPE(cxAnimateItem, cxAction)
{
    CX_SETTER(cxAnimateItem, id);
    CX_SETTER(cxAnimateItem, key);
    CX_SETTER(cxAnimateItem, texture);
    CX_SETTER(cxAnimateItem, time);
    CX_SETTER(cxAnimateItem, flipx);
    CX_SETTER(cxAnimateItem, flipy);
}
CX_INIT(cxAnimateItem, cxObject)
{
    this->time = 0;
    this->value = 0;
    this->flipx = false;
    this->flipy = false;
}
CX_FREE(cxAnimateItem, cxObject)
{
    CX_RELEASE(this->id);
    CX_RELEASE(this->key);
    CX_RELEASE(this->texture);
}
CX_TERM(cxAnimateItem, cxObject)

CX_INLINE cxAnimateItem cxAnimateItemClone(cxAnimateItem this)
{
    cxAnimateItem cp = CX_CREATE(cxAnimateItem);
    CX_RETAIN_SET(cp->id, this->id);
    CX_RETAIN_SET(cp->key, this->key);
    CX_RETAIN_SET(cp->texture, this->texture);
    cp->time = this->time;
    return cp;
}

CX_INLINE cxHashKey cxAnimateItemKey(cxAnimateItem this)
{
    CX_ASSERT(this->key != NULL, "must set cxAnimateItem key");
    cxConstChars key = this->id != NULL ? cxStringBody(this->id) : cxStringBody(this->key);
    return cxHashStrKey(key);
}

static cxAnimateItem cxAnimateItemGet(cxAnimate this,cxArray items, cxAny any, cxInt index)
{
    if(CX_NAME_OF(any) == cxAnimateItemTypeName){
        return any;
    }
    if(CX_NAME_OF(any) != cxStringTypeName){
        return NULL;
    }
    cxConstChars key = cxStringBody(any);
    cxAnimateItem frame = cxHashGet(this->frames, cxHashStrKey(key));
    if(frame == NULL){
        return NULL;
    }
    //update frame at array index
    frame = cxAnimateItemClone(frame);
    cxArrayUpdate(items, frame, index);
    return frame;
}

/* 1 dt 2 dt 3 dt 4 dt 5 dt 6 dt 7 dt 8*/
static void CX_METHOD(cxAnimate,Init)
{
    this->index = 0;
    cxFloat time = cxActionGetTime(this);
    cxArray items = cxAnimateGetGroup(this,this->name);
    CX_ASSERT(items != NULL, "group name not found");
    cxInt num = cxArrayLength(items);
    cxFloat dt = this->time / (cxFloat)num;
    cxFloat value = 0;
    for (cxInt i = 0; i < num; i++) {
        cxAny any = cxArrayAtIndex(items, i);
        cxAnimateItem item = cxAnimateItemGet(this, items, any, i);
        CX_ASSERT_TYPE(item, cxAnimateItem);
        item->value = value;
        time += item->time;
        value += dt + item->time;
    }
    cxActionSetTime(this, time);
}

static void CX_METHOD(cxAnimate,Step,cxFloat dt,cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxSprite, view);
    cxArray items = cxAnimateGetGroup(this,this->name);
    CX_ASSERT(items != NULL, "group name not found ");
    cxFloat elapsed = cxActionGetTimeElapsed(this);
    for (cxInt i = this->index; i < cxArrayLength(items); i++) {
        cxAnimateItem item = cxArrayAtIndex(items,i);
        CX_ASSERT_TYPE(item, cxAnimateItem);
        if(elapsed < item->value){
            break;
        }
        if(item->texture != NULL){
            cxSpriteSetTexture(view, item->texture);
        }
        if(item->key != NULL){
            cxSpriteSetTextureKey(view, cxStringBody(item->key));
        }
        cxSpriteSetFlip(view, item->flipx, item->flipy);
        CX_EVENT_FIRE(this, onFrame);
        //set to next frame
        this->index = i + 1;
    }
}

static void CX_METHOD(cxAnimate,Reset)
{
    this->index = 0;
    cxActionSetTime(this, this->time);
    CX_SUPER(cxAction, this, Reset, CX_MT(void));
}

static cxBool CX_METHOD(cxAnimate,Exit)
{
    this->Repeat --;
    if(this->Repeat == 0){
        return true;
    }
    CX_CALL(this, Reset, CX_MT(void));
    return false;
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
    cxAnimateItem frame = cxJsonMakeObject(item);
    CX_ASSERT_TYPE(frame, cxAnimateItem);
    cxHashKey key = cxAnimateItemKey(frame);
    cxHashSet(this->frames, key, frame);
    CX_JSON_ARRAY_EACH_END(frames, item)
}
CX_SETTER_DEF(cxAnimate, repeat)
{
    this->Repeat = cxJsonToInt(value, this->Repeat);
}
CX_SETTER_DEF(cxAnimate, groups)
{
    cxJson groups = value;
    CX_JSON_OBJECT_EACH_BEG(groups, item)
    CX_ASSERT(cxJsonIsArray(item), "must is array");
    cxString key = cxStringConstChars(itemKey);
    cxArray items = cxAnimateGetGroup(this,key);
    cxJson frames = cxJsonToArray(item);
    CX_JSON_ARRAY_EACH_BEG(frames, ats)
    if(cxJsonIsObject(ats)){
        cxAnimateItem frame = cxJsonMakeObject(ats);
        CX_ASSERT_TYPE(frame, cxAnimateItem);
        cxArrayAppend(items, frame);
        continue;
    }
    if(cxJsonIsString(ats)){
        cxConstChars ik = cxJsonToConstChars(ats);
        cxString frameKey = cxStringConstChars(ik);
        cxArrayAppend(items, frameKey);
        continue;
    }
    CX_JSON_ARRAY_EACH_END(frames, ats)
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

CX_TYPE(cxAnimate, cxAction)
{
    CX_SETTER(cxAnimate, repeat);
    CX_SETTER(cxAnimate, time);
    CX_SETTER(cxAnimate, name);
    CX_SETTER(cxAnimate, frames);
    CX_SETTER(cxAnimate, groups);
    
    CX_MSET(cxAnimate, Init);
    CX_MSET(cxAnimate, Step);
    CX_MSET(cxAnimate, Exit);
    CX_MSET(cxAnimate, Reset);
}
CX_INIT(cxAnimate, cxAction)
{
    this->Repeat = CX_FOREVER;
    this->groups = CX_ALLOC(cxHash);
    this->frames = CX_ALLOC(cxHash);
}
CX_FREE(cxAnimate, cxAction)
{
    CX_RELEASE(this->frames);
    CX_RELEASE(this->groups);
    CX_RELEASE(this->name);
    CX_EVENT_RELEASE(this->onFrame);
}
CX_TERM(cxAnimate, cxAction)

void cxAnimateRunGroup(cxAny pav,cxConstChars name)
{
    CX_ASSERT_THIS(pav, cxAnimate);
    CX_RETURN(!cxConstCharsOK(name));
    //group name not exists return
    CX_RETURN(!cxHashHas(this->groups, cxHashStrKey(name)));
    CX_CALL(this, Reset, CX_MT(void));
    CX_RETAIN_SWAP(this->name, cxStringConstChars(name));
}











