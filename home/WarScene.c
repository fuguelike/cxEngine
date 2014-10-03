//
//  WarScene.c
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "WarScene.h"

CX_OBJECT_TYPE(WarScene, cxScroll)
{
    
}
CX_OBJECT_INIT(WarScene, cxScroll)
{
    
}
CX_OBJECT_FREE(WarScene, cxScroll)
{
    
}
CX_OBJECT_TERM(WarScene, cxScroll)

WarScene WarSceneCreate()
{
    cxLoader loader = cxLoaderCreate("war.json");
    CX_ASSERT_VALUE(cxLoaderGetRoot(loader), WarScene, scene);
    return scene;
}