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
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>

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
//    Node node = CX_TYPE_CAST(Node, pnode);
//    CX_LOGGER("%p new idx=%f idx.y=%f",pnode,node->idx.x,node->idx.y);
}

void MapSortNode(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    this->isSort = true;
}

static void MapAppendNode(cxAny pmap,cxSpatial sp,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    Node snode = CX_TYPE_CAST(Node, node);
    cxViewAppend(this, snode);
    cxSpatialInsert(sp, snode);
}

static void MapRemoveNode(cxSpatial sp,cxAny node)
{
    cxSpatialRemove(sp, node);
    cxViewRemove(node);
}

void MapAppendAttack(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapAppendNode(map, map->attacks, node);
}

void MapAppendDefence(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapAppendNode(map, map->defences, node);
}

void MapAppendBlock(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapAppendNode(map, map->blocks, node);
}

void MapRemoveAttack(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapRemoveNode(map->attacks, node);
}

void MapRemoveDefence(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapRemoveNode(map->defences, node);
}

void MapRemoveBlock(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapRemoveNode(map->blocks, node);
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
    
    cxSize2f size = cxSize2fv(engine->winsize.w * 1.2f, 0);
    size.h = size.w * 0.75f;
    global.unitSize = cxSize2fv(size.w/global.unitNum.x, size.h/global.unitNum.y);
    cxViewSetSize(this, size);
    cxViewSetSize(this->bullet, size);

    
    //test
    cxSpriteSetTextureURL(this, "bg3.png");
    cxBoxTex2f tex = cxBoxTex2fDefault();
    cxVec2f pos = cxVec2fv(size.w / 4, size.h / 4);
    cxColor4f color = cxColor4fv(1, 1, 1, 1);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex, color);
    
    cxBoxTex2f tex1 = cxBoxTex2fFlip(tex, true, false);
    pos = cxVec2fv(-size.w / 4, size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex1, color);

    cxBoxTex2f tex2 = cxBoxTex2fFlip(tex, true, true);
    pos = cxVec2fv(-size.w / 4, -size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex2, color);

    cxBoxTex2f tex3 = cxBoxTex2fFlip(tex, false, true);
    pos = cxVec2fv(size.w / 4, -size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex3, color);

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
    if(item->type == cxTouchTypeUp && item->movement < 30){
        cxVec2f idx = MapPosToIdx(this, cpos);
        CX_LOGGER("fight mode selected:%f %f",idx.x,idx.y);
        //test
        if(this->tag == 1){
            Attack node = AttackCreate(this, cxSize2fv(1, 1), idx);
            NodeSetLife(node, 20000);
            NodeSetPower(node, 20);
            NodeAppend(node);
        }else if(this->tag == 2){
            Defence node = DefenceCreate(this, cxSize2fv(2, 2), idx);
            cxViewSetColor(node, cxRED);
            NodeSetLife(node, 200);
            NodeAppend(node);
        }else if(this->tag == 3){
            Wall node = WallCreate(this, cxSize2fv(1, 1), idx);
            cxViewSetColor(node, cxBLACK);
            NodeSetLife(node, 100);
            NodeAppend(node);
        }
        return true;
    }
    return false;
}

static void mapSubType(cxAny dst,cxAny src)
{
    CX_ASSERT_THIS(dst, Map);
    this->tag = cxViewTag(src);
}

static cpBB NodeIndexBB(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return cpBBNew(this->idx.x, this->idx.y, this->idx.x + this->size.w, this->idx.y + this->size.h);
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

static cxBool MapIsAppend(cxAny pstar,cxVec2i *idx)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    Map map = info->map;
    cxVec2f index = cxVec2fv(idx->x, idx->y);
    //如果索引超出最大范围
    if(!MapIsValidIdx(map, index)){
        return false;
    }
    //如果索引不在人眼视角范围
    
    Node node = MapNode(map, idx->x, idx->y);
    if(node == NULL){
        return true;
    }
    if(node == info->snode || node == info->dnode){
        return true;
    }
    //如果是阻挡类型
    if(node->type == NodeTypeBlock && !NodeIsDie(node)){
        return false;
    }
    return true;
}

CX_OBJECT_TYPE(Map, cxAtlas)
{
    CX_PROPERTY_SETTER(Map, mode);
}
CX_OBJECT_INIT(Map, cxAtlas)
{
    //当选中按钮执行
    cxMessageAppend(this, mapSubType, "selectSubType");
    
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onUpdate, MapUpdate);
    this->mode = MapModeNormal;
    this->isSort = true;
    this->astar = CX_ALLOC(cxAStar);
    cxAStarSetType(this->astar, cxAStarTypeA8);
    CX_METHOD_SET(this->astar->IsAppend, MapIsAppend);
    
    this->defences  = cxSpatialAlloc(NodeIndexBB);
    this->attacks   = cxSpatialAlloc(NodeIndexBB);
    this->blocks    = cxSpatialAlloc(NodeIndexBB);
    
    this->bullet = CX_CREATE(cxView);
    cxViewAppend(this, this->bullet);
    
    this->nodes = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(Map, cxAtlas)
{
    cxMessageRemove(this);
    CX_RELEASE(this->bullet);
    CX_RELEASE(this->blocks);
    CX_RELEASE(this->astar);
    CX_RELEASE(this->attacks);
    CX_RELEASE(this->defences);
    CX_RELEASE(this->nodes);
}
CX_OBJECT_TERM(Map, cxAtlas)

cxAnyArray MapSearchPath(cxAny snode,cxAny dnode)
{
    Map map = NodeMap(snode);
    MapSearchInfo info = {map, snode, dnode};
    cxVec2i s = NodeIndex(snode);
    cxVec2i d = NodeIndex(dnode);
    return cxAStarRun(map->astar, s, d, &info);
}

void MapDelNode(cxAny pmap,cxInt x,cxInt y)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    cxHashDel(this->nodes, key);
}

void MapSetNode(cxAny pmap,cxInt x,cxInt y,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    cxHashSet(this->nodes, key, node);
}

cxAny MapNode(cxAny pmap,cxInt x,cxInt y)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    return cxHashGet(this->nodes, key);
}
                            
cxAny MapItem(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    if(!MapIsValidIdx(this, idx)){
        return NULL;
    }
    return MapNode(pmap, idx.x, idx.y);
}

void MapFillNode(cxAny pmap,cxVec2i idx,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
    Node snode = node;
    cxSize2i size = NodeSize(node);
    if(!MapIsValidIdx(this, snode->idx)){
        goto setnewpos;
    }
    //清除旧位置上的建筑
    cxVec2i curr = NodeIndex(node);
    for(cxInt x = curr.x; x < curr.x + size.w; x ++){
        for (cxInt y = curr.y; y < curr.y + size.h; y++) {
            MapDelNode(this, x, y);
        }
    }
setnewpos:
    //将建筑设置到新位置
    for(cxInt x = idx.x; x < idx.x + size.w; x ++){
        for (cxInt y = idx.y; y < idx.y + size.h; y++) {
            MapSetNode(this, x, y, node);
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











