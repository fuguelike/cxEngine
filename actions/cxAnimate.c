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

CX_OBJECT_DEF(cxAnimateItem, cxObject)
    cxTexture texture;
    cxString key;
    cxFloat delay;
    cxFloat time;
CX_OBJECT_END(cxAnimateItem, cxObject)

CX_OBJECT_TYPE(cxAnimateItem, cxAction)
{
    
}
CX_OBJECT_INIT(cxAnimateItem, cxObject)
{
    
}
CX_OBJECT_FREE(cxAnimateItem, cxObject)
{
    CX_RELEASE(this->key);
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxAnimateItem, cxObject)

//texture.json?blue.png
void cxAnimateItemAppend(cxArray list,cxConstChars file,cxConstChars keyfmt,cxFloat delay)
{
    cxAnimateItem this = CX_CREATE(cxAnimateItem);
    if(file != NULL){
        CX_RETAIN_SET(this->texture, cxTextureFactoryLoadFile(file));
    }
    if(keyfmt != NULL){
        this->key = cxStringAllocChars(keyfmt);
    }
    this->delay = delay;
    cxArrayAppend(list, this);
}

//texture.json
void cxAnimateAppendSeries(cxArray list,cxConstChars file,cxConstChars keyfmt,cxInt from,cxInt to,cxFloat delay)
{
    for(cxInt i = from; i <= to ; i++){
        cxConstChars sfile = CX_CONST_STRING(file,i);
        cxUrlPath path = cxUrlPathParse(sfile);
        cxConstChars skey = keyfmt != NULL ? CX_CONST_STRING(keyfmt, i) : NULL;
        cxAnimateItemAppend(list, path->path, path->count == 2 ? path->key : skey, delay);
    }
}

static void cxAnimateInit(cxAny pav)
{
    cxAnimate this = pav;
    this->index = 0;
    this->cxAction.time = this->time;
    cxFloat dt = this->time / (cxFloat)cxArrayLength(this->list);
    cxFloat i = 0;
    CX_ARRAY_FOREACH(this->list, ele){
        cxAnimateItem item = cxArrayObject(ele);
        this->cxAction.time += item->delay;
        i += dt  + item->delay;
        item->time = i;
    }
}

static void cxAnimateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxAnimate this = pav;
    cxInt i = 0;
    CX_RETURN(this->index >= cxArrayLength(this->list));
    CX_ARRAY_FOREACH(this->list, e){
        i++;
        cxAnimateItem item = cxArrayObject(e);
        if(this->cxAction.timeElapsed > item->time){
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
        cxConstChars texture = cxJsonConstChars(item, "texture");
        cxConstChars key = cxJsonConstChars(item, "key");
        cxInt from = cxJsonInt(item, "from", 0);
        cxInt to = cxJsonInt(item, "to", 0);
        cxFloat delay = cxJsonDouble(item, "delay", 0);
        if((to - from) > 0){
            cxAnimateAppendSeries(this->list, texture, key, from, to, delay);
        }else{
            cxAnimateItemAppend(this->list, texture, key, delay);
        }
    }
    CX_JSON_ARRAY_EACH_END(frames, item)
}

CX_OBJECT_TYPE(cxAnimate, cxAction)
{
    CX_PROPERTY_SETTER(cxAnimate, time);
    CX_PROPERTY_SETTER(cxAnimate, frames);
}
CX_OBJECT_INIT(cxAnimate, cxAction)
{
    CX_METHOD_SET(this->cxAction.Init, cxAnimateInit);
    CX_METHOD_SET(this->cxAction.Step, cxAnimateStep);
    CX_METHOD_SET(this->cxAction.Reset, cxAnimateReset);
    this->list = CX_ALLOC(cxArray);
    this->cached = true;
}
CX_OBJECT_FREE(cxAnimate, cxAction)
{
    CX_EVENT_RELEASE(this->onFrame);
    CX_RELEASE(this->list);
}
CX_OBJECT_TERM(cxAnimate, cxAction)

cxAnimate cxAnimateCreate(cxFloat time,cxArray list)
{
    cxAnimate this = CX_CREATE(cxAnimate);
    CX_RETAIN_SWAP(this->list, list);
    this->time = time;
    return this;
}










