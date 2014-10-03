//
//  FightMap.h
//  cxCore
//
//  Created by xuhua on 9/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_FightMap_h
#define Home_FightMap_h

#include <views/cxScroll.h>
#include "Map.h"

CX_C_BEGIN

CX_OBJECT_DEF(FightMap, Map)
    
CX_OBJECT_END(FightMap, Map)

cxBool FightMapInit(cxAny pmap);

CX_OBJECT_DEF(FightScene, cxScroll)
    cxAny fightMap;      //战斗地图
CX_OBJECT_END(FightScene, cxScroll)

FightScene FightSceneCreate();

CX_C_END

#endif
