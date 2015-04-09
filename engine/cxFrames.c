//
//  cxcxFrames.c
//  cxEngineStatic
//
//  Created by xuhua on 1/15/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <views/cxAtlas.h>
#include "cxFrames.h"

void cxFramesParseTable(cxFrameTable *ft,cxJson json)
{
    cxFrameTableClear(ft);
    CX_JSON_ARRAY_EACH_BEG(json, item)
    if(cxJsonArrayLength(item) != 2){
        continue;
    }
    cxInt a = cxJsonToInt(cxJsonArrayAt(item, 0), -1);
    cxInt l = cxJsonToInt(cxJsonArrayAt(item, 1), -1);
    if(a >= 0 && l >= 0){
        cxFrameTableSet(ft, a, l);
    }
    CX_JSON_ARRAY_EACH_END(ts, item)
}

static cxBool FrameUseTable(cxFrames fs,cxFrame frame,cxAny pview,const cxFrameTable *ft)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    for(cxInt i=0;i<ft->num;i++){
        cxInt atlasIdx = ft->atlasIdx[i];
        cxInt layerIdx = ft->layerIdx[i];
        if(atlasIdx > this->capacity){
            CX_WARN("atlas index out");
            continue;
        }
        if(layerIdx < 0 || layerIdx >= cxArrayLength(frame->coords)){
            CX_WARN("layer index out");
            continue;
        }
        cxBoxPoint *bp = cxAnyArrayAt(frame->boxes, layerIdx, cxBoxPoint);
        cxAtlasUpdateAt(this, atlasIdx, bp);
    }
    return true;
}

static cxBool FrameUseStart(cxFrames fs,cxFrame frame,cxAny pview,cxInt start)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    for(cxInt i=0;i<cxArrayLength(frame->coords);i++){
        if((start + i) > this->capacity){
            CX_WARN("atlas index out");
            continue;
        }
        cxBoxPoint *bp = cxAnyArrayAt(frame->boxes, i, cxBoxPoint);
        cxAtlasUpdateAt(this, start + i, bp);
    }
    return true;
}

cxInt cxAtlasSetFrames(cxAny pview,cxAny pfs,cxInt frameIdx,const cxFrameTable *ft,cxInt start)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    CX_ASSERT_VALUE(pfs, cxFrames, fs);
    cxFramesInit(fs, pview, cxVec2fv(0, 0));
    cxInt num = cxArrayLength(fs->Frames);
    frameIdx = ((frameIdx % num) + num) % num;
    cxFrame frame = cxArrayAtIndex(fs->Frames, frameIdx);
    if(fs->table.num > 0 ){
        FrameUseTable(fs, frame, this, &fs->table);
    }else if(ft != NULL && ft->num > 0){
        FrameUseTable(fs, frame, this, ft);
    }else{
        FrameUseStart(fs, frame, this, start);
    }
    return frameIdx;
}

CX_TYPE(cxFrame, cxObject)
{
    
}
CX_INIT(cxFrame, cxObject)
{
    this->coords = CX_ALLOC(cxArray);
    this->boxes = cxAnyArrayAlloc(cxBoxPoint);
}
CX_FREE(cxFrame, cxObject)
{
    CX_RELEASE(this->coords);
    CX_RELEASE(this->boxes);
}
CX_TERM(cxFrame, cxObject)

void cxFramesAppendLayers(cxAny fs,cxAny pview,cxInt i,cxVec2f offset,cxBool flipx,cxBool flipy)
{
    CX_ASSERT_THIS(fs, cxFrames);
    CX_ASSERT_TYPE(pview, cxAtlas);
    cxTexture texture = cxSpriteGetTexture(pview);
    CX_ASSERT(texture != NULL, "texture error");
    cxSize2f size = cxViewGetSize(pview);
    cxColor4f color = cxViewGetColor(pview);
    cxChar key[CX_HASH_MAX_KEY]={0};
    cxFrame frame = CX_ALLOC(cxFrame);
    offset = cxVec2fAdd(offset, this->offset);
    for (cxInt j=0; j<this->layers; j++) {
        key[snprintf(key, CX_HASH_MAX_KEY, "%d.%d.png",i,j)]='\0';
        cxTexCoord coord = cxTextureCoord(texture, key);
        cxBoxPoint bp={0};
        cxTexCoordSetBoxPoint(pview, &bp, offset, size, color, flipx, flipy, coord);
        cxAnyArrayAppend(frame->boxes, &bp);
        cxArrayAppend(frame->coords, coord);
    }
    cxArrayAppend(this->Frames, frame);
    CX_RELEASE(frame);
}

void cxFramesReset(cxAny fs)
{
    CX_ASSERT_THIS(fs, cxFrames);
    cxArrayClear(this->Frames);
    this->isInit = false;
}

cxBool cxFramesInit(cxAny fs,cxAny pview,cxVec2f offset)
{
    CX_ASSERT_THIS(fs, cxFrames);
    CX_ASSERT_VALUE(pview, cxAtlas, atlas);
    if(this->from < 0 || this->count < 0 || this->layers < 0){
        CX_ERROR("cxFrames args error");
        return false;
    }
    if(this->isInit){
        return true;
    }
    if(this->layers > atlas->capacity){
        cxAtlasSetCapacity(atlas, this->layers);
    }
    CX_CALL(this, OnInit, CX_M(void,cxAny,cxVec2f),atlas,offset);
    this->isInit = true;
    return true;
}
CX_SETTER_DEF(cxFrames, from)
{
    this->from = cxJsonToInt(value, -1);
}
CX_SETTER_DEF(cxFrames, count)
{
    this->count = cxJsonToInt(value, -1);
}
CX_SETTER_DEF(cxFrames, layers)
{
    this->layers = cxJsonToInt(value, -1);
}
CX_SETTER_DEF(cxFrames, offset)
{
    this->offset = cxJsonToVec2f(value, this->offset);
}
CX_SETTER_DEF(cxFrames, flipx)
{
    this->isFlipX = cxJsonToBool(value, false);
}
CX_SETTER_DEF(cxFrames, flipy)
{
    this->isFlipY = cxJsonToBool(value, false);
}
//"table":[[1,0],[2,1]]
CX_SETTER_DEF(cxFrames, table)
{
    if(!cxJsonIsArray(value)){
        return;
    }
    cxFramesParseTable(&this->table, value);
}
CX_METHOD_DEF(cxFrames, OnInit, void, cxAny pview, cxVec2f offset)
{
    for (cxInt i = this->from; i < (this->from + this->count); i++) {
        cxFramesAppendLayers(this, pview, i, offset, this->isFlipX, this->isFlipY);
    }
}
CX_TYPE(cxFrames, cxObject)
{
    CX_SETTER(cxFrames, table);
    CX_SETTER(cxFrames, offset);
    CX_SETTER(cxFrames, from);
    CX_SETTER(cxFrames, count);
    CX_SETTER(cxFrames, layers);
    CX_SETTER(cxFrames, flipx);
    CX_SETTER(cxFrames, flipy);
    
    CX_METHOD(cxFrames, OnInit);
}
CX_INIT(cxFrames, cxObject)
{
    this->setbox = true;
    this->Frames = CX_ALLOC(cxArray);
    this->layers = 1;
    this->count = 1;
}
CX_FREE(cxFrames, cxObject)
{
    CX_RELEASE(this->Frames);
}
CX_TERM(cxFrames, cxObject)

void cxFramesSetAttr(cxAny fs,cxInt from,cxInt count,cxInt layers)
{
    CX_ASSERT_THIS(fs, cxFrames);
    this->from = from;
    this->count = count;
    this->layers = layers;
}

cxFrames cxFramesCreate(cxInt from,cxInt count,cxInt layers)
{
    cxFrames this = CX_CREATE(cxFrames);
    cxFramesSetAttr(this, from, count, layers);
    return this;
}

cxFrames cxFramesClone(cxFrames fs)
{
    cxFrames this = CX_CREATE(cxFrames);
    this->from = fs->from;
    this->count = fs->count;
    this->layers = fs->layers;
    this->table = fs->table;
    this->offset = fs->offset;
    return this;
}

void cxFramesSet(cxFrames fs,cxArray frames)
{
    CX_RETAIN_SWAP(fs->Frames, frames);
}

cxFrames cxFramesLoad(cxConstChars url)
{
    cxLoader loader = cxLoaderCreate(url);
    CX_ASSERT(loader != NULL && loader->Root != NULL, "load frames %s failed",url);
    return loader->Root;
}