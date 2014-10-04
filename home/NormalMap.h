//
//  NormalMap.h
//  cxCore
//  非战斗地图
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_NormalMap_h
#define Home_NormalMap_h

#include <views/cxScroll.h>
#include "Map.h"

CX_C_BEGIN

CX_OBJECT_DEF(NormalMap, Map)
    cxVec2f prevIdx;
    cxAny currNode;         //当前选中的node
    cxAny dnode;            //按下时选中的node
    cxAny unode;            //up时选中的node 
CX_OBJECT_END(NormalMap, Map)

cxBool NormalMapInit(cxAny pmap);

CX_OBJECT_DEF(NormalScene, cxScroll)
    cxAny normalMap;      //战斗地图
CX_OBJECT_END(NormalScene, cxScroll)

NormalScene NormalSceneCreate();

CX_C_END

#endif
