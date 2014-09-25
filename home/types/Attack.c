//
//  Attack.c
//  Home
//
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//
#include <Move.h>
#include <Map.h>
#include "Attack.h"

static void AttackOnDegrees(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(Node, cxActionView(pav));
    cxConstChars n = CX_CONST_STRING("%d.png",this->index);
    cxSpriteSetTextureURL(node, n);
}

static void AttackAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxHash bindes = cxViewBindes(this);
    FixArray items = {0};
    CX_HASH_FOREACH(bindes, ele, tmp){
        cxNumber data = CX_TYPE_CAST(cxNumber, ele->any);
        //为设定标记无法攻击
        if(!data->value.bv){
            continue;
        }
        //获取bind的目标
        Node target = cxHashElementKeyToAny(ele);
        //模拟攻击一下
        NodeAddLife(target, -this->power);
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
        MapRemoveNode(node);
    }
}

static void AttackMoveOnExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = cxActionView(this);
    FixArray items = {0};
    //移动到指定位置后bind攻击
    cxHash bindes = cxViewBindes(node);
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取bind的目标
        cxNumber data = CX_TYPE_CAST(cxNumber, ele->any);
        Node target = cxHashElementKeyToAny(ele);
        cxFloat d = kmVec2DistanceBetween(&node->idx, &target->idx);
        if(d < node->range.max){
            data->value.bv = true;
            continue;
        }
        //未到达攻击范围需要继续寻路
        FixArrayAppend(items, target);
    }
    //未到达攻击距离需要继续寻路,所以解除bind
    for(cxInt i=0; i < items.number; i++){
        Node target = CX_TYPE_CAST(Node, items.items[i]);
        cxViewUnBind(node, target);
    }
}

static void AttackAttackNode(cxAny pview,cxAny node,cxAnyArray points)
{
    CX_ASSERT_THIS(pview, Attack);
    //show path
    Map map = NodeMap(this);
    cxList subviews = cxViewSubViews(map);
    CX_LIST_FOREACH(subviews, ele){
        cxView tmp = ele->any;
        if(tmp->tag == 1001){
            cxViewRemove(tmp);
        }
    }
    CX_ASTAR_POINTS_FOREACH(points, idx){
        cxVec2f p = cxVec2fv(idx->x, idx->y);
        cxVec2f pos = MapIdxToPos(map, p);
        cxSprite sp = cxSpriteCreateWithURL("bullet.json?shell.png");
        cxViewSetColor(sp, cxYELLOW);
        cxViewSetPos(sp, pos);
        cxViewSetSize(sp, cxSize2fv(10, 10));
        cxViewSetTag(sp, 1001);
        cxViewAppend(map, sp);
    }
    //路劲算法移动到攻击点
    Move m = MoveCreate(node, points);
    CX_EVENT_APPEND(m->OnDegrees, AttackOnDegrees);
    CX_EVENT_APPEND(CX_TYPE(cxAction, m)->onExit, AttackMoveOnExit);
    cxViewAppendAction(this, m);
    
    cxViewBind(this, node, cxNumberBool(false));
}


//搜索算法
static void AttackSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Attack);
    Map map = NodeMap(this);
    cxHash bindes = cxViewBindes(this);
    //如果同时攻击数量到达
    if(cxHashLength(bindes) >= this->attackNum){
        return;
    }
    //动态搜索一个最近的防御单位
    Node node = MapNearestQuery(this, cxRange2fv(0, 200),NodeTypeDefence , NodeSubTypeNone);
    if(node == NULL){
        return;
    }
    //搜索路径
    cxAny block = MapSearchPath(this, node);
    //无阻挡物
    if(block == NULL){
        AttackAttackNode(this, node, MapSearchPoints(map));
        return;
    }
    cxViewSetColor(block, cxRED);
    //搜索阻挡物体
    cxAny ret = MapSearchPath(this,block);
    CX_ASSERT(ret == NULL, "block item search path failed");
    AttackAttackNode(this, block, MapSearchPoints(map));
}

CX_OBJECT_TYPE(Attack, Node)
{
    
}
CX_OBJECT_INIT(Attack, Node)
{
    this->Node.type = NodeTypeAttack;
    this->attackNum = 1;
    NodeSetRange(this, cxRange2fv(0, 2));
    NodeSetAttackRate(this, 0.5f);
    cxSpriteSetTextureURL(this, "bg1.png");
    CX_METHOD_SET(this->Node.Search, AttackSearch);
    CX_METHOD_SET(this->Node.Attack, AttackAttack);
}
CX_OBJECT_FREE(Attack, Node)
{

}
CX_OBJECT_TERM(Attack, Node)

Attack AttackCreate(cxAny map, cxSize2f size,cxVec2f pos)
{
    Attack this = CX_CREATE(Attack);
    NodeInit(this, map, size, pos);
    NodeSearchRun(this);
    return this;
}