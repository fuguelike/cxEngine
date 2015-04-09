//
//  cxcxFrames.h
//  cxEngine
//
//  Created by xuhua on 1/15/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFrames_h
#define cxEngine_cxFrames_h

#include "cxTexture.h"

CX_C_BEGIN

#define MAX_CXFRAME_SLICE_SIZE    8

CX_DEF(cxFrame, cxObject)
    cxArray coords;
    cxAnyArray boxes;
CX_END(cxFrame, cxObject)

//将slice帧显示到atlas索引
typedef struct{
    cxInt num;  //显示slice数量
    cxInt atlasIdx[MAX_CXFRAME_SLICE_SIZE];
    cxInt layerIdx[MAX_CXFRAME_SLICE_SIZE];
}cxFrameTable;

#define cxFrameTableMake()        &(cxFrameTable){.num=0}
#define cxFrameTableSet(f,a,l)    do{(f)->atlasIdx[(f)->num]=a;(f)->layerIdx[(f)->num]=l;(f)->num++;}while(0)
#define cxFrameTableClear(f)      (f)->num = 0

void cxFramesParseTable(cxFrameTable *ft,cxJson json);

CX_DEF(cxFrames, cxObject)
    cxBool setbox;      //是否计算box
    cxFrameTable table; //映射表
    cxArray Frames;     //帧序列
    cxInt from;         //从索引
    cxInt count;        //数量
    cxInt layers;       //切片数量
    cxVec2f offset;     //偏移位置
    cxBool isInit;      //是否初始化
    cxBool isFlipX;     //X flip
    cxBool isFlipY;     //Y flip
CX_END(cxFrames, cxObject)

CX_INLINE cxInt cxFramesGetFrom(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxFrames);
    return this->from;
}

CX_INLINE cxInt cxFramesGetCount(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxFrames);
    return this->count;
}

CX_INLINE cxInt cxFramesGetLayers(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxFrames);
    return this->layers;
}

cxFrames cxFramesClone(cxFrames fs);

void cxFramesReset(cxAny fs);

void cxFramesSet(cxFrames fs,cxArray frames);

cxInt cxAtlasSetFrames(cxAny pview,cxAny pfs,cxInt frameIdx,const cxFrameTable *ft,cxInt start);

cxFrames cxFramesLoad(cxConstChars url);

void cxFramesAppendLayers(cxAny fs,cxAny pview,cxInt i,cxVec2f offset,cxBool flipx,cxBool flipy);

cxBool cxFramesInit(cxAny fs,cxAny pview,cxVec2f offset);

void cxFramesSetAttr(cxAny fs,cxInt from,cxInt count,cxInt layers);

cxFrames cxFramesCreate(cxInt from,cxInt count,cxInt layers);

CX_C_END

#endif
