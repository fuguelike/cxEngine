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
    cxSize2f size = cxViewSize(this);
    pos.y += (size.h - map->unitSize.h)/2.0f;
    return cxTilePosToIdx(pos, map->unitSize);
}

static cxBool NodeTouch(cxAny pview,cxTouch *touch)
{
    Node this = pview;
    cxVec2f pos;
    if(!cxViewHitTest(pview, touch->current, &pos)){
        return false;
    }
    if(touch->type != cxTouchTypeDown){
        return false;
    }
    if(!cxPointsContainPoint(this->box, pos)){
        return false;
    }
    cxVec2f idx = NodePosToIdx(this, pos);
    return MapSelectedNode(this->map, this, idx);
}

CX_OBJECT_TYPE(Node, cxSprite)
{
    
}
CX_OBJECT_INIT(Node, cxSprite)
{
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
    if(idx.x < 1 || (idx.x + size.w + 1) > map->unitNum.x){
        return false;
    }
    if(idx.y < 1 || (idx.y + size.h + 1) > map->unitNum.y){
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
    MapSetNode(map, cxVec2iv(idx.x, idx.y), this);
    this->idx = idx;
    this->curr = idx;
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