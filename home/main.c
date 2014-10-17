//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <engine/cxGroup.h>
#include <views/cxLoading.h>
#include "Map.h"
#include "Node.h"
#include "Button.h"
#include "Range.h"
#include "Move.h"
#include "Bullet.h"
#include "FightMap.h"
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>
#include <types/Longer.h>
#include <types/Flyable.h>
#include "WarMap.h"
#include "NormalMap.h"

void cxEngineType(cxEngine engine)
{
    //基本类型
    CX_TYPE_REG(Map);
    CX_TYPE_REG(Node);
    CX_TYPE_REG(Button);
    CX_TYPE_REG(Range);
    CX_TYPE_REG(Move);
    //战役地图
    CX_TYPE_REG(WarMapUnit);
    CX_TYPE_REG(WarMap);
    CX_TYPE_REG(WarScene);
    //战斗地图
    CX_TYPE_REG(FightMap);
    CX_TYPE_REG(FightScene);
    //普通地图
    CX_TYPE_REG(NormalMap);
    CX_TYPE_REG(NormalScene);
    //单位类型
    CX_TYPE_REG(Bullet);
    CX_TYPE_REG(Attack);
    CX_TYPE_REG(Defence);
    CX_TYPE_REG(Wall);
    CX_TYPE_REG(Flyable);
    CX_TYPE_REG(Longer);
}


static cxLabelTTF progress = NULL;

static void loadingFightSceneStart(cxAny sender)
{
//    CX_ASSERT_THIS(sender, cxLoading);
}

static void loadingFightSceneStep(cxAny sender)
{
    CX_ASSERT_THIS(sender, cxLoading);
    cxLabelTTFSetText(progress, UTF8("%d/%d",this->Index + 1,this->Step));
}

static void loadingFightSceneExit(cxAny sender)
{
//    CX_ASSERT_THIS(sender, cxLoading);
    
    cxAny scene = FightSceneCreate();//NormalSceneCreate();
    cxWindowPushView(scene);
}

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesSize(cxSize2fv(2048, 1536));
#ifndef NDEBUG
    cxEngineSetIsShowBorder(true);
#endif
    //战斗用动画管理器
    cxGroupAppend(FIGHT_ACTION_GROUP, 1.0f);
}
void cxEngineMain(cxEngine engine)
{
    //全局数据初始化
    GlobalInit(engine);
    
    progress = cxLabelTTFCreate(UTF8("100/100"), NULL, 52);

    cxLoading loader = CX_CREATE(cxLoading);
    cxViewAppend(loader, progress);
    cxLoadingSetStep(loader, 20);
    CX_SET(cxLoading, loader, onStart, loadingFightSceneStart);
    CX_SET(cxLoading, loader, onStep, loadingFightSceneStep);
    CX_SET(cxLoading, loader, onExit, loadingFightSceneExit);
    cxLoadingStart(loader);
}

void cxEngineFree(cxEngine engine)
{
    
}
