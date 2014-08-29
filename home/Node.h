//
//  Node.h
//  cxCore
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Node_h
#define Home_Node_h

#include <views/cxSprite.h>

CX_C_BEGIN

CX_OBJECT_DEF(Node, cxSprite)
    cxAny map;
    cxSize2f size;      //占用的格子数
    cxVec2f idx;        //左下角格子坐标
    cxAnyArray box;
    cxVec2f curr;       //当前所在位置
    cxBool canSelected; //可选择
    cxBool isSelected;  //是否悬选中
    cxBool isValidIdx;  //是否在有效的位置
    //nodes element
    cxListElement *element;
CX_OBJECT_END(Node, cxSprite)

cxVec2i NodeIndex(cxAny node);

cxSize2i NodeSize(cxAny node);

//从当前坐标转换为索引坐标
cxVec2f NodePosToIdx(cxAny pview,cxVec2f pos);

cxVec2f NodeCurrIdx(cxAny pview);
//设置并放置
void NodeSetIdx(cxAny node,cxVec2f idx);

//重置到旧位置
void NodeResetPosition(cxAny node);

//检测新索引是否可以放置
cxBool NodeIdxIsValid(cxAny node,cxVec2f curr);

//移动成功返回true
cxBool NodeSetPosition(cxAny node,cxVec2f idx);

void NodeSetSize(cxAny node,cxSize2f size);

void NodeInit(cxAny node,cxSize2f size,cxVec2f pos);

cxAny NodeCreate(cxAny map);

CX_C_END

#endif
