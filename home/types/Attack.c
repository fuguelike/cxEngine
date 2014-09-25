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

//设置动画帧索引
static void AttackOnDegrees(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Attack attack = CX_TYPE_CAST(Attack, cxActionView(pav));
    attack->index = this->index;
    CX_EVENT_FIRE(attack, onIndex);
    
    //Test
    cxConstChars n = CX_CONST_STRING("%d.png",this->index);
    cxSpriteSetTextureURL(attack, n);
}

static void AttackAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Attack);
    cxHash bindes = cxViewBindes(this);

    CX_HASH_FOREACH(bindes, ele, tmp){
        //未设定标记无法攻击
        if(!cxNumberToBool(ele->any)){
            continue;
        }
        //获取攻击目标
        Node target = cxHashElementKeyToAny(ele);

        NodeAddLife(target, -this->Node.power);
        CX_LOGGER("target life = %d",target->life);
        
        this->target = target;
        CX_EVENT_FIRE(this, onAttack);
    }
}

static void AttackMoveOnExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node attacker = cxActionView(this);
    //移动到指定位置后bind攻击
    cxHash bindes = cxViewBindes(attacker);
    CX_HASH_FOREACH(bindes, ele, tmp){
        cxNumber data = CX_TYPE_CAST(cxNumber, ele->any);
        Node target = cxHashElementKeyToAny(ele);
        //目标死了移除
        if(NodeIsDie(target)){
            cxViewUnBind(attacker, target);
            continue;
        }
        //开始攻击
        cxNumberSetBool(data, true);
    }
}

static void AttackNode(cxAny pview,cxAny target,cxAnyArray points)
{
    CX_ASSERT_THIS(pview, Attack);
    //显示测试路径点
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
    Move m = MoveCreate(target, points);
    CX_EVENT_APPEND(m->OnDegrees, AttackOnDegrees);
    CX_EVENT_APPEND(CX_TYPE(cxAction, m)->onExit, AttackMoveOnExit);
    cxViewAppendAction(this, m);
    //bind node use bool data
    cxViewBind(this, target, cxNumberBool(false));
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
    Node target = MapNearestQuery(this, cxRange2fv(0, 200),NodeTypeDefence , NodeSubTypeNone);
    if(target == NULL){
        return;
    }
    //搜索路径
    cxAny block = NULL;
    cxBool ret = MapSearchPath(this, target, &block);
    //寻路成功
    if(ret){
        AttackNode(this, target, MapSearchPoints(map));
        return;
    }
    //没发现阻挡物
    if(block == NULL){
        return;
    }
    //搜索阻挡物体
    if(MapSearchPath(this,block, NULL)){
        AttackNode(this, block, MapSearchPoints(map));
    }
}

CX_OBJECT_TYPE(Attack, Node)
{
    
}
CX_OBJECT_INIT(Attack, Node)
{
    this->Node.type = NodeTypeAttack;
    this->Node.body = 0.3f;
    this->attackNum = 1;
    //近身攻击00
    NodeSetRange(this, cxRange2fv(0, 0));
    NodeSetAttackRate(this, 0.5f);
    cxSpriteSetTextureURL(this, "bg1.png");
    CX_METHOD_SET(this->Node.Search, AttackSearch);
    CX_METHOD_SET(this->Node.Attack, AttackAttack);
}
CX_OBJECT_FREE(Attack, Node)
{
    CX_EVENT_RELEASE(this->onIndex);
    CX_EVENT_RELEASE(this->onAttack);
}
CX_OBJECT_TERM(Attack, Node)

Attack AttackCreate(cxAny map, cxSize2f size,cxVec2f pos)
{
    Attack this = CX_CREATE(Attack);
    NodeInit(this, map, size, pos);
    NodeSearchRun(this);
    return this;
}