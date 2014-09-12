//
//  Node.c
//  Home
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <algorithm/cxTile.h>
#include <views/cxScroll.h>
#include "Node.h"
#include "Map.h"

cxVec2f NodePosToIdx(cxAny pview,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxSize2f vsize = cxViewSize(this);
    pos.y -= (vsize.h / 2.0f - (vsize.h / (2.0f * this->size.h)));
    return MapPosToIdx(map, pos);
}

static cxBool NodeSelected(Node this)
{
    Map map = this->map;
    CX_LIST_FOREACH(map->nodes, ele){
        if(ele->any == this){
            cxViewSetColor(ele->any, cxYELLOW);
        }else{
            cxViewSetColor(ele->any, cxRED);
        }
    }
    map->node = this;
    return true;
}

static cxBool NodeTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Node);
    CX_RETURN(points->number != 1,false);
    cxTouchItem item = points->items[0];
    Map map = this->map;
    if(!this->canSelected){
        return false;
    }
    cxVec2f cpos;
    cxBool hited = cxViewHitTest(pview, item->position, &cpos) && cxPointsContainPoint(this->box, cpos);
    if(item->type == cxTouchTypeMove &&  map->node == this && this->isTouch){
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
        NodeSetPosition(this, idx);
        this->start = vpos;
        return true;
    }
    if(item->type == cxTouchTypeDown){
        this->isTouch = hited;
        this->start = cxViewPosition(this);
        if(hited){
            cxViewBringFront(this);
        }
        return false;
    }
    if(item->type == cxTouchTypeUp && item->movement < 20){
        this->isSelected = hited;
        if(this->isSelected && map->node != NULL){
            NodeResetPosition(map->node);
        }
        if(this->isSelected){
            return NodeSelected(this);
        }
        return false;
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

cxVec2i NodeIndex(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return cxVec2iv(this->idx.x, this->idx.y);
}

cxSize2i NodeSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return cxSize2iv(this->size.w, this->size.h);
}

void NodeResetPosition(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    CX_RETURN(this->isValidIdx);
    NodeSetPosition(this, this->idx);
    this->isValidIdx = true;
}

cxBool NodeSetPosition(cxAny pview,cxVec2f idx)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    this->curr = idx;
    idx.x += (this->size.w - 1.0f)/2.0f;
    idx.y += (this->size.h - 1.0f)/2.0f;
    cxViewSetPos(this, MapIdxToPos(map, idx));
    return true;
}

void NodeSetSize(cxAny pview,cxSize2f size)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxSize2f vsize = cxSize2fv(map->unitSize.w * size.w, map->unitSize.h * size.h);
    cxViewSetSize(this, vsize);
    this->size = size;
    cxAnyArrayClean(this->box);
    cxAnyArrayAppend(this->box, &(cxVec2fv(vsize.w/2.0f, 0)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(0, vsize.h/2.0f)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(-vsize.w/2.0f, 0)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(0, -vsize.h/2.0f)));
}

cxBool NodeIdxIsValid(cxAny pview,cxVec2f curr)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxVec2i idx = cxVec2iv(curr.x, curr.y);
    cxSize2i size = NodeSize(this);
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

void NodeUpdateIdx(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    NodeSetIdx(pview, this->curr);
}

void NodeSetIdx(cxAny pview,cxVec2f idx)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxVec2i nidx = cxVec2iv(idx.x, idx.y);
    cxVec2i oidx = NodeIndex(this);
    if(cxVec2iEqu(nidx, oidx)){
        return;
    }
    MapSetNode(map, nidx, this);
    this->idx = idx;
    this->curr = idx;
    this->isValidIdx = true;
    MapNodeOnNewIdx(map, this);
}

void NodeInit(cxAny pview,cxSize2f size,cxVec2f pos,NodeType type)
{
    CX_ASSERT_THIS(pview, Node);
    NodeSetSize(this, size);
    NodeSetIdx(this, pos);
    NodeSetPosition(this, pos);
    this->type = type;
}

cxAny NodeCreate(cxAny map)
{
    Node this = CX_CREATE(Node);
    this->map = map;
    return this;
}