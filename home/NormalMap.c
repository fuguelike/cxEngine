//
//  NormalMap.c
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "NormalMap.h"
#include <types/Wall.h>
#include <types/Defence.h>

static cxBool NormalMapTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, NormalMap);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    cxVec2f cpos;
    if(!cxViewHitTest(this, item->position, &cpos)){
        return false;
    }
    if(item->type == cxTouchTypeUp){
        cxVec2f index = MapPosToFloat(this, cpos);
        this->currNode = MapHitNode(this, index, NodeTypeAll);
        if(this->currNode == NULL){
            return false;
        }
    }
    return false;
}

CX_OBJECT_TYPE(NormalMap, Map)
{
    
}
CX_OBJECT_INIT(NormalMap, Map)
{
    MapSetMode(this, MapModeNormal);
    CX_SET(cxView, this, Touch, NormalMapTouch);
}
CX_OBJECT_FREE(NormalMap, Map)
{
    
}
CX_OBJECT_TERM(NormalMap, Map)

cxBool NormalMapInit(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, NormalMap);
    if(!MapInit(pmap)){
        return false;
    }
    cxAny node1 = WallCreate(this, cxVec2iv(8, 8));
    MapAppendNode(node1);
    
    cxAny node2 = DefenceCreate(this, cxVec2iv(20, 20));
    MapAppendNode(node2);
    return true;
}

CX_OBJECT_TYPE(NormalScene, cxScroll)
{
    
}
CX_OBJECT_INIT(NormalScene, cxScroll)
{
    
}
CX_OBJECT_FREE(NormalScene, cxScroll)
{
    
}
CX_OBJECT_TERM(NormalScene, cxScroll)

NormalScene NormalSceneCreate()
{
    cxLoader loader = cxLoaderCreate("normal.json");
    CX_ASSERT_VALUE(cxLoaderGetRoot(loader), NormalScene, scene);
    scene->normalMap = cxLoaderGet(loader, "map");
    CX_ASSERT_TYPE(scene->normalMap, NormalMap);
    if(!NormalMapInit(scene->normalMap)){
        CX_ERROR("init normal map error");
        return NULL;
    }
    return scene;
}