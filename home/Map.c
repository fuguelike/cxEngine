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
//    Node node = CX_TYPE_CAST(Node, pnode);
//    CX_LOGGER("%p new idx=%f idx.y=%f",pnode,node->idx.x,node->idx.y);
}

void MapSortNode(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    this->isSort = true;
}

void MapAppendAttack(cxAny pmap,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    Node snode = CX_TYPE_CAST(Node, node);
    cxViewAppend(this, snode);
    cxSpatialInsert(this->attacks, snode);
}

void MapAppendDefence(cxAny pmap,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    Node snode = CX_TYPE_CAST(Node, node);
    cxViewAppend(this, snode);
    cxSpatialInsert(this->defences, snode);
}

void MapRemoveAttack(cxAny pmap,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSpatialRemove(this->attacks, node);
    cxViewRemove(node);
}

void MapRemoveDefence(cxAny pmap,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSpatialRemove(this->defences, node);
    cxViewRemove(node);
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

#include <actions/cxFollow.h>

static void shellExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    //target 被 sp击中
    Node target = CX_TYPE_CAST(Node, this->target);
    cxHash binded = cxViewBinded(target);
    cxLong tag = cxActionTag(pav);
    if(cxHashHas(binded, cxHashLongKey(tag))){
        Node firer = (Node)tag;
        CX_LOGGER("%p",firer);
    }
    

    cxSprite sp = CX_TYPE_CAST(cxSprite, this->cxAction.view);
    
    //隐藏子弹
    cxViewSetVisible(sp, false);
    cxViewRemove(sp);
}

static void NodeDefenceAttackRun(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxHash bindes = cxViewBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        //制造子弹
        cxSprite sp = cxSpriteCreateWithURL("shell.png");
        cxViewSetSize(sp, cxSize2fv(20, 20));
        cxViewAppend(this->map, sp);
        //获取bind的目标
        cxView v = cxHashElementKeyToAny(ele);
        //
        cxFollow f = cxFollowCreate(500, v);
        //设置开火者
        cxActionSetTag(f, (cxLong)this);
        CX_EVENT_APPEND(f->cxAction.onExit, shellExit);
        cxViewAppendAction(sp, f);
    }
}

static void NodeDefenceSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxVec2f pos = cxViewPosition(this);
    NodeNearestInfo info = NodeNearest(map->attacks, pos, 10000 , NodeTypeAttack, NodeSubTypeNone);
    if(info.node != NULL){
        //bind目标
        CX_METHOD_SET(this->Attack, NodeDefenceAttackRun);
        cxViewBind(this, info.node, NULL);
        NodeAttackRun(this, 0.3f);
        NodePauseSearch(this);
    }
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
    
    {
        Node node = CX_CREATE(Node);
        NodeInit(node, this, cxSize2fv(2, 2),cxVec2fv(20, 20),NodeTypeDefence);
        cxSpriteSetTextureURL(node, "bg1.png");
        cxViewSetColor(node, cxRED);
        NodeSetLife(node, 1000);
        MapAppendDefence(this, node);
        CX_METHOD_SET(node->Search, NodeDefenceSearch);
        NodeSearchRun(node, 0.1f);
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

static void NodeOnAngle(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(Node, this->cxAction.view);
    cxInt index = 0;
    cxFloat angle = MoveAngleIndex(this,&index);
    
    cxConstChars n = CX_CONST_STRING("%d.png",index);
    cxSpriteSetTextureURL(node, n);
    cxViewSetDegrees(node, angle);
}

//士兵搜索算法
static void NodeAttackSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxVec2f pos = cxViewPosition(this);
    NodeNearestInfo info = NodeNearest(map->defences, pos, 10000 , NodeTypeNone, NodeSubTypeNone);
    if(info.node != NULL){
        Node node = info.node;
        Move m = CX_CREATE(Move);
        MoveAppendPoint(m, this->idx);
        for(cxFloat a = 0; a < 360; a+=22.5f){
            cxInt x = 20 * sinf(kmDegreesToRadians(a)) + 20;
            cxInt y = 20 * cosf(kmDegreesToRadians(a)) + 20;
            MoveAppendPoint(m, cxVec2fv(x, y));
        }
//        for(cxFloat a = 0; a < 360; a+=10){
//            cxInt x = 20 * sinf(kmDegreesToRadians(a)) + 20;
//            cxInt y = 20 * cosf(kmDegreesToRadians(a)) + 20;
//            MoveAppendPoint(m, cxVec2fv(x, y));
//        }
        MoveAppendPoint(m, info.idx);
        MoveSetTarget(m, node);
        CX_EVENT_APPEND(m->OnAngle, NodeOnAngle);
        
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
        Node node = CX_CREATE(Node);
        NodeInit(node, this, cxSize2fv(2, 2),cxVec2fv(idx.x, idx.y),NodeTypeAttack);
        NodeSetSubType(node, NodeSubTypeSoldier);
        cxSpriteSetTextureURL(node, "bg1.png");
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











