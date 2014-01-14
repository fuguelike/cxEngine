//
//  cxAnimate.c
//  cxEngine
//
//  Created by xuhua on 11/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <views/cxSprite.h>
#include <textures/cxTextureFactory.h>
#include <core/cxTexture.h>
#include <core/cxActionRoot.h>
#include <core/cxUtil.h>
#include <core/cxUrlPath.h>
#include "cxAnimate.h"

CX_OBJECT_DEF(cxAnimateItem, cxObject)
    cxTexture texture;
    cxString key;
    cxFloat delay;
    cxFloat time;
CX_OBJECT_END(cxAnimateItem)

CX_OBJECT_INIT(cxAnimateItem, cxObject)
{
    
}
CX_OBJECT_FREE(cxAnimateItem, cxObject)
{
    CX_RELEASE(this->key);
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxAnimateItem, cxObject)

void cxAnimateItemAppend(cxArray list,cxConstChars file,cxConstChars key,cxFloat delay,cxBool gcache)
{
    cxAnimateItem this = CX_CREATE(cxAnimateItem);
    if(file != NULL){
        this->texture = gcache ? cxTextureFactoryLoadFile(file) : cxTextureCreate(file);
        CX_ASSERT(this->texture != NULL, "%s load failed", file);
        CX_RETAIN(this->texture);
    }
    if(key != NULL){
        this->key = cxStringAllocChars(key);
    }
    this->delay = delay;
    cxArrayAppend(list, this);
}

static void cxAnimateInit(cxAny pav)
{
    cxAnimate this = pav;
    this->index = 0;
    this->super.duration = this->duration;
    cxFloat dt = this->duration / (cxFloat)cxArrayLength(this->list);
    cxFloat i = 0;
    CX_ARRAY_FOREACH(this->list, ele){
        cxAnimateItem item = cxArrayObject(ele);
        this->super.duration += item->delay;
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
        if(this->super.durationElapsed > item->time){
            continue;
        }
        if(this->index == i){
            break;
        }
        if(item->texture != NULL){
            cxSpriteSetTexture(this->super.view, item->texture);
        }
        if(item->texture != NULL && item->key != NULL){
            cxSpriteSetTextureKey(this->super.view, cxStringBody(item->key), false);
        }
        this->index = i;
        CX_EVENT_FIRE(this, onFrame);
        break;
    }
}

static void cxAnimateXMLAppend(cxArray list,cxConstChars file,cxConstChars key,cxInt from,cxInt to,cxBool cache)
{
    for(cxInt i = from; i <= to ; i++){
        cxConstChars sfile = CX_CONST_STRING(file,i);
        cxUrlPath path = cxUrlPathParse(sfile);
        if(path->count == 1){
            cxAnimateItemAppend(list, sfile, NULL, 0, cache);
        }else if(path->count == 2){
            cxAnimateItemAppend(list, path->path, path->key, 0, cache);
        }
    }
}

static void cxAnimateReadAttr(cxReaderAttrInfo *info)
{
    cxActionReadAttr(info);
    cxAnimate this = info->object;
    
    this->cached = cxXMLReadBoolAttr(info, "cxAnimate.cache", true);
    
    int depth = xmlTextReaderDepth(info->reader);
    while(xmlTextReaderRead(info->reader) && depth != xmlTextReaderDepth(info->reader)){
        if(xmlTextReaderNodeType(info->reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(info->reader);
        if(!ELEMENT_IS_TYPE(cxFrame)){
            continue;
        }
        cxConstChars file = cxXMLAttr(info->reader,"file");
        cxConstChars key = cxXMLAttr(info->reader,"key");
        cxInt from = cxXMLReadIntAttr(info, "from", 0);
        cxInt to = cxXMLReadIntAttr(info, "to", 0);
        if(from > 0 && to > 0){
            cxAnimateXMLAppend(this->list, file, key, from, to, this->cached);
        }else if(file != NULL){
            cxUrlPath path = cxUrlPathParse(file);
            cxFloat delay = cxXMLReadFloatAttr(info, "delay", 0);
            cxAnimateItemAppend(this->list, path->path, path->count >= 2 ? path->key : NULL, delay, this->cached);
        }
    }
    this->duration = this->super.duration;
    cxXMLAppendEvent(info, this, cxAnimate, onFrame);
}

static void cxAnimateReset(cxAny pav)
{
    cxAnimate this = pav;
    CX_EVENT_RELEASE(this->onFrame);
}

CX_OBJECT_INIT(cxAnimate, cxAction)
{
    cxObjectSetReadAttrFunc(this, cxAnimateReadAttr);
    CX_METHOD_OVERRIDE(this->super.Init, cxAnimateInit);
    CX_METHOD_OVERRIDE(this->super.Step, cxAnimateStep);
    CX_METHOD_OVERRIDE(this->super.Reset, cxAnimateReset);
    this->list = CX_ALLOC(cxArray);
    this->cached = true;
}
CX_OBJECT_FREE(cxAnimate, cxAction)
{
    CX_EVENT_RELEASE(this->onFrame);
    CX_RELEASE(this->list);
}
CX_OBJECT_TERM(cxAnimate, cxAction)

cxAnimate cxAnimateCreate(cxFloat duration,cxArray list)
{
    cxAnimate this = CX_CREATE(cxAnimate);
    CX_RETAIN_SWAP(this->list, list);
    this->duration = duration;
    return this;
}










