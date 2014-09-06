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

static void cxAnimateInit(cxAny pav)
{
    cxAnimate this = pav;
    CX_ASSERT(CX_INSTANCE_OF(this->cxAction.view, cxSprite), "cxAnimate action view must is cxSprite");
    this->index = 0;
    this->cxAction.time = this->time;
    cxArray list = cxAnimateGroup(this,NULL);
    cxFloat dt = this->time / (cxFloat)cxArrayLength(list);
    cxFloat i = 0;
    CX_ARRAY_FOREACH(list, ele){
        cxAnimateItem item = cxArrayObject(ele);
        this->cxAction.time += item->time;
        i += dt  + item->time;
        item->value = i;
    }
}

static void cxAnimateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxAnimate this = pav;
    cxArray list = cxAnimateGroup(this,NULL);
    cxInt i = 0;
    CX_RETURN(this->index >= cxArrayLength(list));
    CX_ARRAY_FOREACH(list, e){
        i++;
        cxAnimateItem item = cxArrayObject(e);
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
            cxSpriteSetTextureKey(this->cxAction.view, cxStringBody(item->key), false);
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
    cxArray list = cxAnimateGroup(this,CX_ANIMATE_DEFAULT_GROUP);
    CX_JSON_ARRAY_EACH_BEG(frames, item)
    {
        cxObjectCreateResult ret = cxObjectCreateBegin(item);
        CX_ASSERT(CX_INSTANCE_OF(ret.object, cxAnimateItem), "type error");
        cxArrayAppend(list, ret.object);
        cxObjectCreateEnd(&ret);
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
        cxArray list = cxAnimateGroup(this,key);
        cxJson frames = cxJsonToArray(item);
        CX_JSON_ARRAY_EACH_BEG(frames, ats)
        {
            if(cxJsonIsNumber(ats)){
                cxInt idx = cxJsonToInt(ats, -1);
                cxAny frame = cxAnimateItemAt(this, idx);
                CX_CONTINUE(frame == NULL);
                cxArrayAppend(list, frame);
            }else{
                cxObjectCreateResult ret = cxObjectCreateBegin(ats);
                CX_ASSERT(CX_INSTANCE_OF(ret.object, cxAnimateItem), "type error");
                cxArrayAppend(list, ret.object);
                cxObjectCreateEnd(&ret);
            }
        }
        CX_JSON_ARRAY_EACH_END(frames, ats)
    }
    CX_JSON_OBJECT_EACH_END(groups, item)
}
CX_SETTER_DEF(cxAnimate, key)
{
    cxConstChars key = cxJsonToConstChars(value);
    if(key != NULL){
        cxAnimateSetGroupName(this, key);
    }
}

//from default group get item
cxAnimateItem cxAnimateItemAt(cxAny pav,cxInt idx)
{
    cxArray list = cxAnimateGroup(pav, CX_ANIMATE_DEFAULT_GROUP);
    CX_ASSERT(list != NULL, "group %s not exists",CX_ANIMATE_DEFAULT_GROUP);
    if(idx < 0 || idx >= cxArrayLength(list)){
        return NULL;
    }
    return cxArrayAtIndex(list, idx);
}

cxArray cxAnimateGroup(cxAny pav,cxConstChars name)
{
    cxAnimate this = pav;
    if(name == NULL){
        name = cxStringBody(this->groupName);
    }
    cxArray list = cxHashGet(this->groups, cxHashStrKey(name));
    if(list == NULL){
        list = CX_ALLOC(cxArray);
        cxHashSet(this->groups, cxHashStrKey(name), list);
        CX_RELEASE(list);
    }
    return list;
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
    this->groupName = cxStringAllocChars(CX_ANIMATE_DEFAULT_GROUP);
}
CX_OBJECT_FREE(cxAnimate, cxAction)
{
    CX_RELEASE(this->groupName);
    CX_RELEASE(this->groups);
    CX_EVENT_RELEASE(this->onFrame);
}
CX_OBJECT_TERM(cxAnimate, cxAction)

void cxAnimateSetGroupName(cxAny pav,cxConstChars name)
{
    cxAnimate this = pav;
    cxActionReset(this);
    cxString str = cxStringConstChars(name);
    CX_RETAIN_SWAP(this->groupName, str);
}

cxAnimate cxAnimateCreate(cxFloat time,cxConstChars name)
{
    cxAnimate this = CX_CREATE(cxAnimate);
    this->time = time;
    if(name == NULL){
        name = CX_ANIMATE_DEFAULT_GROUP;
    }
    cxAnimateSetGroupName(this, name);
    return this;
}










