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

static void SoldierOnAngle(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(Node, this->cxAction.view);
    cxInt index = 0;
    cxFloat angle = AngleToIndex(this->angle, &index);
    cxConstChars n = CX_CONST_STRING("%d.png",index);
    cxSpriteSetTextureURL(node, n);
    cxViewSetDegrees(node, angle);
}

static void SoldierAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    //如果未到达攻击点
    if(!this->isArrive){
        return;
    }
    Map map = NodeMap(this);
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
        CX_METHOD_RUN(node->UnLock, node, this);
        //死去的防御移除
        MapRemoveDefence(map, node);
    }
}

static void SoldierMoveExit(cxAny pav)
{
    Node node = cxActionView(pav);
    node->isArrive = true;
}

static void SoldierMoving(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    //如果目标死亡停止移动
    Node target = CX_TYPE_CAST(Node, this->target);
    if(NodeIsDie(target)){
        cxActionStop(pav);
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
    //动态搜索一个目标
    NodeNearestInfo info = NodeNearest(map->defences, this->Node.idx, cxRange2fv(0, 100), NodeTypeNone, NodeSubTypeNone);
    if(info.node == NULL){
        return;
    }
    Node node = info.node;
    
    //路劲算法移动到攻击点
    Move m = CX_CREATE(Move);
    MoveAppendPoint(m, this->Node.idx);
//    for(cxFloat a = 0; a < 360; a+=22.5f){
//        cxInt x = 20 * sinf(kmDegreesToRadians(a)) + 20;
//        cxInt y = 20 * cosf(kmDegreesToRadians(a)) + 20;
//        MoveAppendPoint(m, cxVec2fv(x, y));
//    }
    MoveAppendPoint(m, node->idx);
    MoveSetTarget(m, node);
    //移动结束时到达攻击点
    CX_EVENT_APPEND(m->cxAction.onExit, SoldierMoveExit);
    CX_EVENT_APPEND(m->OnAngle, SoldierOnAngle);
    CX_EVENT_APPEND(m->OnMoving, SoldierMoving);
    cxViewAppendAction(this, m);
    
    cxViewBind(this, node, NULL);
    //node被this锁定
    CX_METHOD_RUN(node->OnLock,node,this);
}

CX_OBJECT_TYPE(Soldier, Node)
{
    
}
CX_OBJECT_INIT(Soldier, Node)
{
    this->attackNum = 1;
    NodeSetRange(this, cxRange2fv(0, 2));
    NodeSetAttackRate(this, 0.2f);
    cxSpriteSetTextureURL(this, "bg1.png");
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
    return this;
}