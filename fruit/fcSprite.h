//
//  fcSprite.h
//  cxEngine
//
//  Created by xuhua on 3/21/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcSprite_h
#define fruit_fcSprite_h

#include <actions/cxTimer.h>
#include <views/cxSprite.h>
#include "fcDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcSprite, cxSprite)
    //
    cxTimer aiTimer;
    //map list element,移除后设置为NULL
    cxListElement *element;
    //地图指针
    cxAny map;
    //当前运动路线
    fcPath path;
    //坐标
    cxVec2i idx;
    //类型
    fcSpriteType type;
    //攻击目标一个或多个,当sprite死去的时候，记得从所有的murderers的targets中删除
    cxHash targets;
    //盯住我的凶手,当攻击目标时记得把目标的murderers加上自己
    cxHash murderers;
    //组 敌人组和自己组
    fcGroupType group;
    //当 sprite 被 fruit 打中，凶手是 attacker
    CX_METHOD_ALLOC(void, Attacked, cxAny sprite, cxAny fruit,cxAny attacker);
    //sprite 是否能被 attacker 发射的 fruit 攻击
    CX_METHOD_ALLOC(cxBool, IsAttack,cxAny sprite,cxAny fruit,cxAny attacker);
    //获取参与路径计算的值
    CX_METHOD_ALLOC(cxInt, PathValue,cxAny sprite);
    //生命
    cxInt life;
    //移动速度
    cxFloat speed;
CX_OBJECT_END(fcSprite)

void fcSpriteStartAITimer(cxAny this,cxEventFunc aiFunc,cxFloat interval);

//暂停一段ai
void fcSpritePauseTime(cxAny this,cxFloat time);

//暂停ai
void fcSpritePause(cxAny this);

//恢复ai
void fcSpriteResume(cxAny this);

//设置组
void fcSpriteSetGroup(cxAny this,fcGroupType group);

//寻找通向idx的路径
cxBool fcSpriteFindPath(cxAny this,cxVec2i idx);

//获取参与路径搜索的值
cxInt fcSpritePathValue(cxAny this);

void fcSpriteInitIndex(cxAny this, cxVec2i idx);

void fcSpriteMoveTo(cxAny this,cxVec2i idx);

void fcSpriteRemoved(cxAny this);

//获取目标数量
cxInt fcSpriteTargetNumber(cxAny this);

//获取凶手数量
cxInt fcSpriteMurdererNumber(cxAny this);

//s1 指向 s2
void fcSpriteLookAt(cxAny s1,cxAny s2);

//获取两个sprite间的距离
cxFloat fcSpriteDistance(cxAny s1,cxAny s2);

//解除所有关系
void fcSpriteUnset(cxAny this);

//target已经有这个目标
cxBool fcSpriteHasTarget(cxAny this,cxAny target);

//设置目标
void fcSpriteTarget(cxAny this,cxAny target);

void fcSpriteInit(cxAny this,cxAny map);

//创建定时器
cxTimer fcSpriteTimer(cxAny this,cxFloat time);

CX_C_END

#endif
