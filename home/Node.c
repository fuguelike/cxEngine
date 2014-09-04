//
//  Node.c
//  Home
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <algorithm/cxTile.h>
#include "Node.h"
#include "Map.h"

cxVec2f NodePosToIdx(cxAny pview,cxVec2f pos)
{
    Node this = pview;
    Map map = this->map;
    cxSize2f vsize = cxViewSize(this);
    pos.y -= (vsize.h / 2.0f - (vsize.h / (2.0f * this->size.h)));
    return MapPosToIdx(map, pos);
}

static cxBool NodeTouch(cxAny pview,cxTouchItems *fires,cxTouchItems *points)
{
    CX_RETURN(CX_TOUCH_SIZE(points) != 1,false);
    cxTouchItem item = CX_TOUCH_ITEM(fires, 0);
    Node this = pview;
    Map map = this->map;
    //不可选择?
    if(!this->canSelected){
        return false;
    }
    cxVec2f cpos;
    cxBool hited = cxViewHitTest(pview, item->current, &cpos) && cxPointsContainPoint(this->box, cpos);
    Node snode = map->node;
    //如果移动一个选中的node
    if(item->type == cxTouchTypeMove &&  map->node != NULL && this->isSelected){
        cxVec2f vpos = this->start;
        cxVec2f delta = cxViewTouchDelta(pview, item);
        kmVec2Add(&vpos, &vpos, &delta);
        cxVec2f idx = NodePosToIdx(this,vpos);
        this->isValidIdx = NodeIdxIsValid(this, idx);
        if(this->isValidIdx){
            cxViewSetColor(this, cxGREEN);
        }else{
            cxViewSetColor(this, cxGRAY);
        }
        cxVec2i cidx = cxVec2iv(idx.x, idx.y);
        cxVec2i oidx = NodeIndex(map->node);
        if(!cxVec2iEqu(cidx, oidx)){
            NodeSetPosition(this, idx);
        }
        this->start = vpos;
        return true;
    }
    //按下的时候并选中一个node
    if(item->type == cxTouchTypeDown && hited){
        this->start = cxViewPosition(this);
        this->isSelected = true;
        return false;
    }
    //按下得时候没选中node，有之前选中的node,并且不再一个有效的位置上
    if(item->type == cxTouchTypeDown && !hited && snode != NULL && snode == this){
        if(!snode->isValidIdx){
            NodeResetPosition(snode);
        }
        snode->isSelected = false;
        map->node = NULL;
        cxViewSetColor(snode, cxRED);
        return false;
    }
    //如果当前选中的node在按键起来的时候有效
    if(item->type == cxTouchTypeUp && snode != NULL && snode == this && snode->isValidIdx){
        NodeSetIdx(snode, snode->curr);
        return false;
    }
    //如果选中了一个node
    if(item->type == cxTouchTypeUp && this->isSelected && hited){
        CX_LIST_FOREACH(map->nodes, ele){
            Node cnode = ele->any;
            if(cnode == this){
                cxViewSetColor(ele->any, cnode->isValidIdx ? cxYELLOW : cxGRAY);
                cxViewBringFront(map, this);
            }else{
                cnode->isSelected = false;
                cxViewSetColor(ele->any, cxRED);
            }
        }
        map->node = this;
        return true;
    }
    return false;
}

CX_OBJECT_TYPE(Node, cxSprite)
{
    
}
CX_OBJECT_INIT(Node, cxSprite)
{
    this->canSelected = true;
    this->idx = cxVec2fv(-1, -1);
    CX_METHOD_SET(this->cxSprite.cxView.Touch, NodeTouch);
    this->box = cxAnyArrayAlloc(cxVec2f);
}
CX_OBJECT_FREE(Node, cxSprite)
{
    CX_RELEASE(this->box);
}
CX_OBJECT_TERM(Node, cxSprite)

cxVec2i NodeIndex(cxAny node)
{
    Node this = node;
    return cxVec2iv(this->idx.x, this->idx.y);
}

cxSize2i NodeSize(cxAny node)
{
    Node this = node;
    return cxSize2iv(this->size.w, this->size.h);
}

void NodeResetPosition(cxAny node)
{
    Node this = node;
    NodeSetPosition(this, this->idx);
    this->isValidIdx = true;
}

cxBool NodeSetPosition(cxAny node,cxVec2f idx)
{
    Node this = node;
    Map map = this->map;
    this->curr = idx;
    idx.x += (this->size.w - 1.0f)/2.0f;
    idx.y += (this->size.h - 1.0f)/2.0f;
    cxViewSetPos(this, MapIdxToPos(map, idx));
    return true;
}

void NodeSetSize(cxAny node,cxSize2f size)
{
    Node this = node;
    Map map = this->map;
    cxSize2f vsize = cxSize2fv(map->unitSize.w * size.w, map->unitSize.h * size.h);
    cxViewSetSize(node, vsize);
    this->size = size;
    cxAnyArrayClean(this->box);
    cxAnyArrayAppend(this->box, &(cxVec2fv(vsize.w/2.0f, 0)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(0, vsize.h/2.0f)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(-vsize.w/2.0f, 0)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(0, -vsize.h/2.0f)));
}

cxBool NodeIdxIsValid(cxAny node,cxVec2f curr)
{
    Node this = node;
    Map map = this->map;
    cxVec2i idx = cxVec2iv(curr.x, curr.y);
    cxSize2i size = NodeSize(node);
    // 1 = max -1
    if(idx.x < 0 || (idx.x + size.w) > map->unitNum.x){
        return false;
    }
    if(idx.y < 0 || (idx.y + size.h) > map->unitNum.y){
        return false;
    }
    for(cxInt x = idx.x; x < idx.x + size.w; x ++){
        for (cxInt y = idx.y; y < idx.y + size.h; y++) {
            cxAny item = map->items[MapOffsetIdx(map, x, y)];
            if(item != NULL && item != this){
                return false;
            }
        }
    }
    return true;
}

cxVec2f NodeCurrIdx(cxAny pview)
{
    Node this = pview;
    return this->curr;
}

void NodeSetIdx(cxAny node,cxVec2f idx)
{
    Node this = node;
    Map map = this->map;
    cxVec2i nidx = cxVec2iv(idx.x, idx.y);
    cxVec2i oidx = NodeIndex(node);
    if(cxVec2iEqu(nidx, oidx)){
        return;
    }
    MapSetNode(map, nidx, this);
    this->idx = idx;
    this->curr = idx;
    this->isValidIdx = true;
    MapNodeIdxChanged(map, node);
}

void NodeInit(cxAny node,cxSize2f size,cxVec2f pos)
{
    Node this = node;
    NodeSetSize(node, size);
    NodeSetIdx(this, pos);
    NodeSetPosition(node, pos);
}

cxAny NodeCreate(cxAny map)
{
    Node this = CX_CREATE(Node);
    this->map = map;
    return this;
}