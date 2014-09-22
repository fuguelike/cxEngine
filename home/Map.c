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
#include "Move.h"

static cxInt MapSortCmpFunc(cxConstAny lv,cxConstAny rv)
{
    cxListElement *lp = (cxListElement *)lv;
    cxListElement *rp = (cxListElement *)rv;
    cxView v1 = CX_TYPE_CAST(cxView, lp->any);
    cxView v2 = CX_TYPE_CAST(cxView, rp->any);
    return v2->position.y - v1->position.y;
}

void MapNodeOnNewIdx(cxAny pmap,cxAny pnode)
{
//    CX_ASSERT_THIS(pmap, Map);
    Node node = CX_TYPE_CAST(Node, pnode);
    CX_LOGGER("%p new idx=%f idx.y=%f",pnode,node->idx.x,node->idx.y);
}

void MapSortNode(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    this->isSort = true;
}

void MapAppendAttack(cxAny pmap,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
    Node snode = node;
    cxViewAppend(this, snode);
    cxSpatialInsert(this->attacks, snode);
}

void MapAppendDefence(cxAny pmap,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
    Node snode = node;
    cxViewAppend(this, snode);
    cxSpatialInsert(this->defences, snode);
}

void MapRemoveAttack(cxAny pmap,cxAny node)
{
    
}

void MapRemoveDefence(cxAny pmap,cxAny node)
{
    
}

static void MapNodesEach(cxAny ps,cxAny pview,cxAny data)
{
    Node node = pview;
    cxBool *has = data;
    if(!(*has) && node->isSelected){
        *has = true;
    }
    
}
static cxBool hasSelectNode(Map this)
{
    cxBool has = false;
    cxSpatialEach(this->defences, MapNodesEach, &has);
    return has;
}

static cxBool MapEditTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Map);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    Node snode = this->node;
    if(item->type == cxTouchTypeMove){
        return false;
    }
    if(snode == NULL){
        return false;
    }
    if(item->type == cxTouchTypeUp && snode->isValidIdx){
        NodeUpdateIdx(snode);
    }
    cxBool isSelected = hasSelectNode(this);
    if(item->type == cxTouchTypeUp && !isSelected){
        NodeResetPosition(snode);
        cxViewSetColor(snode, cxRED);
        this->node = NULL;
    }
    return false;
}

cxBool MapInit(cxAny pmap,cxJson data)
{
    CX_ASSERT_THIS(pmap, Map);

    cxEngine engine = cxEngineInstance();

    this->items = allocator->calloc(global.unitNum.x * global.unitNum.y,sizeof(cxAny));
    
    cxSize2f size = cxSize2fv(engine->winsize.w * 1.2f, 0);
    size.h = size.w * 0.75f;
    global.unitSize = cxSize2fv(size.w/global.unitNum.x, size.h/global.unitNum.y);
    cxViewSetSize(this, size);
    
    //test
    cxSpriteSetTextureURL(this, "bg1.png");
    for(cxInt x = 0; x < global.unitNum.x; x++){
        for (cxInt y = 0; y < global.unitNum.y; y++) {
            cxVec2f pos = MapIdxToPos(this, cxVec2fv(x, y));
            cxAtlasAppendBoxPoint(this, pos, cxSize2fv(global.unitSize.w - 2, global.unitSize.h - 2), cxBoxTex2fDefault(), cxColor4fv(1, 1, 1, 1));
        }
    }
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(3, 3),cxVec2fv(8, 8),NodeTypeNone);
        cxSpriteSetTextureURL(node, "bg1.png");
        cxViewSetColor(node, cxRED);
        MapAppendDefence(this, node);
    }
    
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(4, 4),cxVec2fv(0, 0),NodeTypeNone);
        cxSpriteSetTextureURL(node, "bg1.png");
        cxViewSetColor(node, cxRED);
        MapAppendDefence(this, node);
    }
    
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(1, 1),cxVec2fv(18, 18),NodeTypeNone);
        cxSpriteSetTextureURL(node, "bg1.png");
        cxViewSetColor(node, cxRED);
        MapAppendDefence(this, node);
    }
    
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(2, 2),cxVec2fv(7, 18),NodeTypeNone);
        cxSpriteSetTextureURL(node, "bg1.png");
        cxViewSetColor(node, cxRED);
        MapAppendDefence(this, node);
    }
    
    //按Y位置排序
    MapSortNode(this);
    return true;
}

static void MapUpdate(cxAny pview)
{
    CX_ASSERT_THIS(pview, Map);
    if(this->isSort){
        cxViewSortWithFunc(this, MapSortCmpFunc);
        this->isSort = false;
    }
}

//士兵搜索算法
static void NodeAttackSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxVec2f pos = cxViewPosition(this);
    NodeNearestInfo info = NodeNearest(map->defences, pos, 10000 , NULL);
    if(info.node != NULL){
        Node node = info.node;
        Move m = CX_CREATE(Move);
        MoveAppendPoint(m, this->idx);
        MoveAppendPoint(m, info.idx);
        cxViewAppendAction(pview, m);
        NodePauseSearch(pview);
    }
}

static cxBool MapFightTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Map);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    cxVec2f cpos;
    if(!cxViewHitTest(pview, item->position, &cpos)){
        return false;
    }
    if(item->type == cxTouchTypeDown){
        this->isSelectUnit = true;
        return false;
    }
    if(item->type == cxTouchTypeUp && item->movement < 10){
        cxVec2f idx = MapPosToIdx(this, cpos);
        CX_LOGGER("fight mode selected:%f %f",idx.x,idx.y);
        //test
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(1, 1),cxVec2fv(idx.x, idx.y),NodeTypeAttack);
        cxSpriteSetTextureURL(node, "bg1.png");
        cxViewSetColor(node, cxORANGE);
        MapAppendAttack(this, node);
        NodeSearchRun(node, SEARCH_DELAY);
        CX_METHOD_SET(node->Search, NodeAttackSearch);
        
        return true;
    }
    return false;
}

CX_SETTER_DEF(Map, mode)
{
    cxConstChars mode = cxJsonToConstChars(value);
    if(cxConstCharsHas(mode, "normal")){
        this->mode = MapModeNormal;
        CX_METHOD_SET(CX_TYPE(cxView, this)->Touch, MapEditTouch);
    }else if(cxConstCharsHas(mode, "fight")){
        this->mode = MapModeFight;
        CX_METHOD_SET(CX_TYPE(cxView, this)->Touch, MapFightTouch);
    }
}

CX_OBJECT_TYPE(Map, cxAtlas)
{
    CX_PROPERTY_SETTER(Map, mode);
}
CX_OBJECT_INIT(Map, cxAtlas)
{
    
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onUpdate, MapUpdate);
    this->mode = MapModeNormal;
    this->isSort = true;
    this->defences = CX_ALLOC(cxSpatial);
    this->attacks = CX_ALLOC(cxSpatial);
}
CX_OBJECT_FREE(Map, cxAtlas)
{
    CX_RELEASE(this->attacks);
    CX_RELEASE(this->defences);
    allocator->free(this->items);
}
CX_OBJECT_TERM(Map, cxAtlas)

cxInt MapOffsetIdx(cxAny pmap,cxInt x,cxInt y)
{
    return y * global.unitNum.x + x;
}
                            
cxAny MapItem(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    if(!MapIsValidIdx(this, idx)){
        return NULL;
    }
    cxInt off = MapOffsetIdx(this, idx.x, idx.y);
    return this->items[off];
}

cxBool MapRemoveNode(cxAny pmap,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
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
    cxSpatialRemove(this->node, n);
    cxViewRemove(node);
    return true;
}

void MapSetNode(cxAny pmap,cxVec2i idx,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
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

cxVec2f MapPosToIdx(cxAny pmap,cxVec2f pos)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewSize(this);
    pos.y += (size.h - global.unitSize.h)/2.0f;
    return cxTilePosToIdx(pos, global.unitSize);
}

cxBool MapIsValidIdx(cxAny pmap,cxVec2f idx)
{
    return idx.x >= 0 && idx.x < global.unitNum.x && idx.y >= 0 && idx.y < global.unitNum.y;
}

cxVec2f MapIdxToPos(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewSize(this);
    cxVec2f pos = cxTileIdxToPos(idx, global.unitSize);
    pos.y -= (size.h - global.unitSize.h)/2.0f;
    return pos;
}











