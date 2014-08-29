//
//  Map.c
//  Home
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <algorithm/cxTile.h>
#include <views/cxLabelTTF.h>
#include <views/cxSprite.h>
#include <engine/cxEngine.h>
#include "Map.h"
#include "Node.h"

static cxBool MapTouch(cxAny pview,cxTouch *touch)
{
    cxEngine engine = cxEngineInstance();
    Map this = pview;
    cxVec2f pos;
    if(!cxViewHitTest(pview, touch->current, &pos)){
        return false;
    }
    cxSize2f msize = cxViewSize(this);
    cxSize2f wsize = engine->winsize;
    if(touch->type == cxTouchTypeDown){
        this->box.r = (msize.w - wsize.w)/2.0f;
        this->box.l = -this->box.r;
        this->box.t = (msize.h - wsize.h)/2.0f;
        this->box.b = -this->box.t;
        return false;
    }
    if(touch->type == cxTouchTypeMove){
        cxVec2f pos = cxViewPosition(this);
        kmVec2Add(&pos, &pos, &touch->delta);
        if(touch->delta.y < 0 && pos.y <= this->box.b){
            pos.y = this->box.b;
        }else if(touch->delta.y > 0 && pos.y >= this->box.t){
            pos.y = this->box.t;
        }
        if(touch->delta.x < 0 && pos.x <= this->box.l){
            pos.x = this->box.l;
        }else if(touch->delta.x > 0 && pos.x >= this->box.r){
            pos.x = this->box.r;
        }
        cxViewSetPos(this, pos);
        return true;
    }
    return false;
}

CX_OBJECT_TYPE(Map, cxAtlas)
{
    
}
CX_OBJECT_INIT(Map, cxAtlas)
{
    CX_METHOD_SET(this->cxAtlas.cxSprite.cxView.Touch, MapTouch);
    //set size
    cxEngine engine = cxEngineInstance();
    this->unitNum = cxVec2iv(20, 20);
    this->items = allocator->calloc(this->unitNum.x * this->unitNum.y,sizeof(cxAny));
    this->nodes = CX_ALLOC(cxList);
    
    cxSize2f size = cxSize2fv(engine->winsize.w * 1.2f, 0);
    size.h = size.w * 0.75f;
    this->unitSize = cxSize2fv(size.w/this->unitNum.x, size.h/this->unitNum.y);
    cxViewSetSize(this, size);
    //
    
    //test
    for(cxInt x = 0; x < this->unitNum.x; x++){
        for (cxInt y = 0; y < this->unitNum.y; y++) {
            cxVec2f pos = MapIdxToPos(this, cxVec2fv(x, y));
            cxSprite sp = cxSpriteCreateWithURL("bg1.png");
            cxViewSetSize(sp, this->unitSize);
            cxViewSetPos(sp, pos);
            cxViewAppend(this, sp);
        }
    }
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(3, 3),cxVec2fv(5, 5));
        cxSpriteSetTextureURL(node, "bg1.png", false);
        cxViewSetColor(node, cxRED);
        MapAppendNode(this, node);
    }
    
//    {
//        Node node = NodeCreate(this);
//        NodeInit(node, cxSize2fv(4, 4),cxVec2fv(0, 0));
//        cxSpriteSetTextureURL(node, "bg1.png", false);
//        cxViewSetColor(node, cxRED);
//        MapAppendNode(this, node);
//    }
//    
//    {
//        Node node = NodeCreate(this);
//        NodeInit(node, cxSize2fv(1, 1),cxVec2fv(18, 18));
//        cxSpriteSetTextureURL(node, "bg1.png", false);
//        cxViewSetColor(node, cxRED);
//        MapAppendNode(this, node);
//    }
}
CX_OBJECT_FREE(Map, cxAtlas)
{
    CX_RELEASE(this->nodes);
    allocator->free(this->items);
}
CX_OBJECT_TERM(Map, cxAtlas)

void MapAppendNode(Map this,cxAny node)
{
    Node snode = node;
    cxViewAppend(this, snode);
    snode->element = cxListAppend(this->nodes, snode);
}

cxInt MapOffsetIdx(Map this,cxInt x,cxInt y)
{
    return y * this->unitNum.x + x;
}
                            
cxAny MapItem(Map this,cxVec2f idx)
{
    cxInt off = MapOffsetIdx(this, idx.x, idx.y);
    if(!MapIsValidIdx(this, idx)){
        return false;
    }
    return this->items[off];
}

cxBool MapRemoveNode(Map this,cxAny node)
{
    CX_ASSERT(node != NULL, "node null");
    Node n = node;
    if(n->map != this){
        return false;
    }
    cxInt off = 0;
    cxSize2i size = NodeSize(node);
    cxVec2i curr = NodeIndex(node);
    for(cxInt x = curr.x; x < curr.x + size.w; x ++){
        for (cxInt y = curr.y; y < curr.y + size.h; y++) {
            off = MapOffsetIdx(this, x, y);
            this->items[off] = NULL;
        }
    }
    n->map = NULL;
    if(n->element != NULL){
        cxListRemove(this->nodes, n->element);
        n->element = NULL;
    }
    cxViewRemoved(node);
    return true;
}

void MapSetNode(Map this,cxVec2i idx,cxAny node)
{
    Node snode = node;
    cxSize2i size = NodeSize(node);
    if(!MapIsValidIdx(this, snode->idx)){
        goto setnewpos;
    }
    //清楚旧位置上的建筑
    cxVec2i curr = NodeIndex(node);
    for(cxInt x = curr.x; x < curr.x + size.w; x ++){
        for (cxInt y = curr.y; y < curr.y + size.h; y++) {
            cxInt off = MapOffsetIdx(this, x, y);
            this->items[off] = NULL;
        }
    }
setnewpos:
    //将建筑设置到新位置
    for(cxInt x = idx.x; x < idx.x + size.w; x ++){
        for (cxInt y = idx.y; y < idx.y + size.h; y++) {
            cxInt off = MapOffsetIdx(this, x, y);
            this->items[off] = node;
        }
    }
}

cxVec2f MapPosToIdx(Map this,cxVec2f pos)
{
    cxSize2f size = cxViewSize(this);
    pos.y += (size.h - this->unitSize.h)/2.0f;
    return cxTilePosToIdx(pos, this->unitSize);
}

cxBool MapIsValidIdx(Map this,cxVec2f idx)
{
    return idx.x >= 0 && idx.x < this->unitNum.x && idx.y >= 0 && idx.y < this->unitNum.y;
}

cxHashKey MapIdxToKey(Map this,cxVec2i idx)
{
    cxInt key = (cxInt)idx.y * this->unitNum.x + (cxInt)idx.x;
    return cxHashIntKey(key);
}

cxVec2f MapIdxToPos(Map this,cxVec2f idx)
{
    cxSize2f size = cxViewSize(this);
    cxVec2f pos = cxTileIdxToPos(idx, this->unitSize);
    pos.y -= (size.h - this->unitSize.h)/2.0f;
    return pos;
}











