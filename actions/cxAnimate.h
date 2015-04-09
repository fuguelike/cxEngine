//
//  cxAnimate.h
//  home
//  atlas animate
//  Created by xuhua on 12/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAnimate_h
#define cxEngine_cxAnimate_h

#include <engine/cxAction.h>
#include <engine/cxFrames.h>

CX_C_BEGIN

CX_DEF(cxAnimate, cxAction)
    CX_FIELD_DEF(cxFloat Time);         //播放时间
    CX_FIELD_DEF(cxFloat FrameTime);    //每帧的播放时间
    CX_FIELD_DEF(cxBool IsLoop);        //循环播放
    CX_FIELD_DEF(cxInt Index);      //0-n
    CX_FIELD_DEF(cxInt Start);
    CX_FIELD_DEF(cxAny Frames);
    CX_FIELD_DEF(cxVec2f Offset);   //偏移位置
    CX_FIELD_DEF(cxRange2i Range);  //设置当前组播放帧范围
    CX_FIELD_DEF(cxBool IsCircle);  //环播放
    cxFrameTable table;             //映射表，优先 Frames->table
    cxFloat delta;
    cxInt number;   //当前播放帧数量
    cxInt from;     //从from帧
    cxInt to;       //到to帧
    cxInt prevIdx;
    cxBool isReverse;   //环播放下的方向
CX_END(cxAnimate, cxAction)

CX_FIELD_SET(cxAnimate, cxBool, IsCircle);

CX_FIELD_GET(cxAnimate, cxRange2i, Range);

CX_INLINE void cxAnimateSetRange(cxAny pthis,const cxRange2i value)
{
    CX_ASSERT_THIS(pthis, cxAnimate);
    CX_RETURN(cxRange2iEqu(this->Range, value));
    this->Range = value;
    cxActionReset(this);
}

CX_INLINE void cxAnimateSetTime(cxAny pthis,const cxFloat value)
{
    CX_ASSERT_THIS(pthis, cxAnimate);
    this->Time = value;
    this->FrameTime = 0;
    cxActionReset(this);
}

CX_INLINE void cxAnimateSetFrame(cxAny pthis,cxInt index)
{
    cxAnimateSetRange(pthis, cxRange2ix(index));
}

CX_INLINE void cxAnimateSetFrameTime(cxAny pthis,const cxFloat value)
{
    CX_ASSERT_THIS(pthis, cxAnimate);
    this->Time = 0;
    this->FrameTime = value;
    cxActionReset(this);
}

CX_FIELD_IMP(cxAnimate, cxVec2f, Offset);
CX_FIELD_SET(cxAnimate, cxBool, IsLoop);
CX_FIELD_GET(cxAnimate, cxInt, Index);
CX_FIELD_SET(cxAnimate, cxInt, Start);
CX_FIELD_IMO(cxAnimate, cxAny, Frames);

void cxAnimateSetFrameTable(cxAny pav,const cxFrameTable *ft);

void cxAnimateFrameTableSet(cxAny pav,cxInt atlasidx,cxInt layeridx);

void cxAnimateFrameTableClear(cxAny pav);

cxAnimate cxAnimateCreate(cxAny fs,cxFloat time);

CX_C_END

#endif
