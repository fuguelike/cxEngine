//
//  Wall.c
//  Home
//
//  Created by xuhua on 9/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <Map.h>
#include "Wall.h"

CX_TYPE(Wall, Node)
{
    
}
CX_INIT(Wall, Node)
{
    NodeSetType(this, NodeCombinedMake(NodeTypeBlock, NodeSubTypeWall));
    NodeSetSize(this, cxSize2iv(1, 1));
    
    cxSpriteSetTextureURL(this, "bg1.png");
    cxViewSetColor(this, cxBLACK);
}
CX_FREE(Wall, Node)
{
    
}
CX_TERM(Wall, Node)

Wall WallCreate(cxAny pmap,cxVec2i pos)
{
    Wall this = CX_CREATE(Wall);
    NodeInit(this, pmap, pos, true);
    return this;
}