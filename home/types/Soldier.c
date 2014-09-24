//
//  Soldier.c
//  Home
//
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//
#include <Move.h>
#include <Map.h>
#include "Soldier.h"

static void SoldierOnDegrees(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(Node, cxActionView(pav));
    cxConstChars n = CX_CONST_STRING("%d.png",this->index);
    cxSpriteSetTextureURL(node, n);
    cxViewSetDegrees(node, this->angle);
}

static void SoldierAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    //如果未到达攻击点
    if(!this->isArrive){
        return;
    }
    cxHash bindes = cxViewBindes(this);
    FixArray items = {0};
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取bind的目标
        Node target = cxHashElementKeyToAny(ele);
        //模拟攻击一下
        NodeAddLife(target, -this->attack);
        CX_LOGGER("Turret life = %d",target->life);
        //死去的目标
        if(NodeIsDie(target)){
            FixArrayAppend(items, target);
            continue;
        }
    }
    //移除死去的
    for(cxInt i=0; i < items.number; i++){
        Node node = CX_TYPE_CAST(Node, items.items[i]);
        cxViewUnBind(this, node);
        //死去的防御移除
        NodeRemove(node);
    }
}

//搜索算法
static void SoldierSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Soldier);
    Map map = NodeMap(this);
    cxHash bindes = cxViewBindes(this);
    //如果同时攻击数量到达
    if(cxHashLength(bindes) >= this->attackNum){
        return;
    }
    this->Node.isArrive = false;
    //动态搜索一个最近的防御单位
    NodeNearestInfo info = NodeNearest(map->defences, this->Node.idx, cxRange2fv(0, 100), NodeTypeDefence, NodeSubTypeNone);
    if(info.node == NULL){
        //搜索一个城墙
        info = NodeNearest(map->blocks, this->Node.idx, cxRange2fv(0, 100), NodeTypeBlock, NodeSubTypeNone);
    }
    if(info.node == NULL){
        return;
    }
    
    Node node = info.node;
    
    //搜索
    
    cxAnyArray points = MapSearchPath(this, node);
    
    //show path
    cxView mp = (cxView)map;
    CX_LIST_FOREACH(mp->subViews, ele){
        cxView tmp = ele->any;
        if(tmp->tag == 1000){
            cxViewRemove(tmp);
        }
    }
    CX_ANY_ARRAY_FOREACH(points, idx, cxVec2i){
        cxVec2f p = cxVec2fv(idx->x, idx->y);
        cxVec2f pos = MapIdxToPos(map, p);
        cxSprite sp = cxSpriteCreateWithURL("shell.png");
        cxViewSetColor(sp, cxRED);
        cxViewSetPos(sp, pos);
        cxViewSetSize(sp, cxSize2fv(10, 10));
        cxViewSetTag(sp, 1000);
        cxViewAppend(map, sp);
    }
    
    //路劲算法移动到攻击点
    Move m = MoveCreate(node, points);
    CX_EVENT_APPEND(m->OnDegrees, SoldierOnDegrees);
    cxViewAppendAction(this, m);
    
    cxViewBind(this, node, NULL);
}

CX_OBJECT_TYPE(Soldier, Node)
{
    
}
CX_OBJECT_INIT(Soldier, Node)
{
    this->attackNum = 1;
    NodeSetRange(this, cxRange2fv(0, 2));
    NodeSetAttackRate(this, 0.5f);
    cxSpriteSetTextureURL(this, "bg1.png");
    
    CX_METHOD_SET(this->Node.Remove, MapRemoveAttack);
    CX_METHOD_SET(this->Node.Append, MapAppendAttack);
    
    CX_METHOD_SET(this->Node.Search, SoldierSearch);
    CX_METHOD_SET(this->Node.Attack, SoldierAttack);
}
CX_OBJECT_FREE(Soldier, Node)
{

}
CX_OBJECT_TERM(Soldier, Node)

Soldier SoldierCreate(cxAny map, cxSize2f size,cxVec2f pos)
{
    Soldier this = CX_CREATE(Soldier);
    NodeInit(this, map, size, pos, NodeTypeAttack, NodeSubTypeSoldier);
    NodeSearchRun(this);
    return this;
}