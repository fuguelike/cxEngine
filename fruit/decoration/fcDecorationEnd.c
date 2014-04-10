//
//  fcDecorationEnd.c
//  fruit
//
//  Created by xuhua on 4/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcDecorationEnd.h"

CX_OBJECT_INIT(fcDecorationEnd, fcDecoration)
{
    
}
CX_OBJECT_FREE(fcDecorationEnd, fcDecoration)
{
    
}
CX_OBJECT_TERM(fcDecorationEnd, fcDecoration)

void fcDecorationEndOnArrive(cxAny pview,cxAny sprite)
{
    fcDecorationEnd this = pview;
    cxVec2i idx = fcSpriteIndex(this);
    CX_LOGGER("%p arrive %d %d",sprite,idx.x,idx.y);
}

void fcDecorationEndInit(cxAny pview,cxAny map)
{
    fcDecorationEnd this = pview;
    fcDecorationInit(this, map, fcDecorationTypeEnd);
}