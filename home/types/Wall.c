//
//  Wall.c
//  Home
//
//  Created by xuhua on 9/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <Map.h>
#include "Wall.h"

CX_OBJECT_TYPE(Wall, Node)
{
    
}
CX_OBJECT_INIT(Wall, Node)
{
    this->Node.type = NodeTypeBlock;
    this->Node.subType = NodeSubTypeWall;
    cxSpriteSetTextureURL(this, "bg1.png");
}
CX_OBJECT_FREE(Wall, Node)
{
    
}
CX_OBJECT_TERM(Wall, Node)

Wall WallCreate(cxAny map,cxSize2f size,cxVec2f pos)
{
    Wall this = CX_CREATE(Wall);
    NodeInit(this, map, size, pos);
    return this;
}