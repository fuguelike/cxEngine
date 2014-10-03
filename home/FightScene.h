//
//  Scene.h
//  Home
//
//  Created by xuhua on 8/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_FightScene_h
#define Home_FightScene_h

#include <views/cxScroll.h>

CX_C_BEGIN

CX_OBJECT_DEF(FightScene, cxScroll)
    cxAny fightMap;      //战斗地图
CX_OBJECT_END(FightScene, cxScroll)

FightScene FightSceneCreate();
CX_C_END

#endif
