//
//  cxAnimate.c
//  home
//  Node帧动画
//  Created by xuhua on 12/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <textures/cxTextureCache.h>
#include <views/cxAtlas.h>
#include <engine/cxEngine.h>
#include "cxAnimate.h"

static cxInt cxAnimateGetNumber(cxAnimate this,cxInt frameNum)
{
    CX_ASSERT_VALUE(this->Frames, cxFrames, fs);
    this->from = ((this->from % frameNum) + frameNum) % frameNum;
    this->to = ((this->to % frameNum) + frameNum) % frameNum;
    cxInt count = abs(this->to - this->from) + 1;
    this->isReverse = this->from > this->to;
    if(this->IsCircle && count > (frameNum/2)){
        count = frameNum - count;
        this->isReverse = !this->isReverse;
    }
    return count;
}

CX_METHOD_DEF(cxAnimate,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxAtlas, view);
    CX_ASSERT_VALUE(this->Frames, cxFrames, fs);
    if(cxFramesInit(this->Frames,view,this->Offset) < 0){
        CX_WARN("init cxAnimate  frames error");
        cxActionStop(this);
        return;
    }
    if(fs->table.num > 0){
        this->table = fs->table;
    }
    cxInt frameNum = cxArrayLength(fs->Frames);
    if(frameNum == 0){
        CX_WARN("cxAnimate frame number == 0");
        cxActionStop(this);
        return;
    }
    this->prevIdx = -1;
    this->from = 0;
    this->to = frameNum - 1;
    //如果设置了range
    if(this->Range.max != INT32_MAX && this->Range.min != INT32_MIN){
        this->from = this->Range.min;
        this->to = this->Range.max;
    }
    this->number = cxAnimateGetNumber(this,frameNum);
    if(this->number == 0){
        CX_WARN("cxAnimate play frame == 0");
        cxActionStop(this);
        return;
    }
    //如果设置帧时间
    if(!cxFloatEqu(this->FrameTime, 0.0f)){
        this->Time = this->FrameTime * (cxFloat)this->number;
    }
    //如果时间为0或播放帧=1
    if(cxFloatEqu(this->Time, 0.0f) || this->number == 1){
        this->Index = cxAtlasSetFrames(view, this->Frames, this->from, &this->table, this->Start);
        cxActionStop(this);
        return;
    }
    this->delta = this->Time/(cxFloat)this->number;
    cxActionSetTime(this, this->Time);
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxAnimate,Step,void,cxFloat dt,cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxAtlas, view);
    cxFloat elapsed = cxActionGetTimeElapsed(this);
    cxInt idx = elapsed / this->delta;
    if(idx >= this->number){
        idx = this->number - 1;
    }
    if(this->prevIdx == idx){
        return;
    }
    this->prevIdx = idx;
    cxInt fidx = this->from + ((this->isReverse)?-idx:+idx);
    this->Index = cxAtlasSetFrames(view, this->Frames, fidx, &this->table, this->Start);
    CX_CALL(this, OnFrame, CX_M(void));
}
CX_METHOD_DEF(cxAnimate,IsExit,cxBool)
{
    CX_CALL(this, OnLoop, CX_M(void));
    this->prevIdx = -1;
    return !this->IsLoop;
}
CX_SETTER_DEF(cxAnimate, frames)
{
    cxFrames frames = cxJsonMakeObject(value);
    CX_ASSERT_TYPE(frames, cxFrames);
    CX_RETAIN_SWAP(this->Frames, frames);
}
CX_SETTER_DEF(cxAnimate, offset)
{
    this->Offset = cxJsonToVec2f(value, this->Offset);
}
CX_SETTER_DEF(cxAnimate, range)
{
    this->Range = cxJsonToRange2i(value, this->Range);
}
//当一帧播放完毕
CX_METHOD_DEF(cxAnimate, OnFrame, void)
{
//    CX_LOGGER("%d",this->Index);
}
//当播放一轮
CX_METHOD_DEF(cxAnimate, OnLoop, void)
{
//    CX_LOGGER("loop");
}
CX_TYPE(cxAnimate, cxAction)
{
    CX_SETTER(cxAnimate, frames);
    CX_SETTER(cxAnimate, range);
    CX_SETTER(cxAnimate, offset);
    
    CX_METHOD(cxAnimate, OnFrame);
    CX_METHOD(cxAnimate, OnLoop);
    CX_METHOD(cxAnimate, Init);
    CX_METHOD(cxAnimate, Step);
    CX_METHOD(cxAnimate, IsExit);
}
CX_INIT(cxAnimate, cxAction)
{
    this->Range = cxRange2iv(INT32_MIN, INT32_MAX);
    this->IsLoop = false;
    this->IsCircle = false;
}
CX_FREE(cxAnimate, cxAction)
{
    CX_RELEASE(this->Frames);
}
CX_TERM(cxAnimate, cxAction)

cxAnimate cxAnimateCreate(cxAny fs,cxFloat time)
{
    CX_ASSERT_VALUE(fs, cxFrames, frames);
    cxAnimate this = CX_CREATE(cxAnimate);
    cxAnimateSetFrames(this, frames);
    cxAnimateSetTime(this, time);
    return this;
}

void cxAnimateSetFrameTable(cxAny pav,const cxFrameTable *ft)
{
    CX_ASSERT_THIS(pav, cxAnimate);
    this->table = *ft;
    cxActionReset(this);
}

void cxAnimateFrameTableSet(cxAny pav,cxInt atlasidx,cxInt layeridx)
{
    CX_ASSERT_THIS(pav, cxAnimate);
    cxFrameTableSet(&this->table, atlasidx, layeridx);
}

void cxAnimateFrameTableClear(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAnimate);
    cxFrameTableClear(&this->table);
}

