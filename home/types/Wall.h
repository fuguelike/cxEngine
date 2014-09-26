//
//  Wall.h
//  Home
//  阻挡进攻类，城墙
//  Created by xuhua on 9/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Wall_h
#define Home_Wall_h

#include <Node.h>

CX_C_BEGIN

CX_OBJECT_DEF(Wall, Node)

CX_OBJECT_END(Wall, Node)

Wall WallCreate(cxAny map,cxSize2f size,cxVec2i pos);

CX_C_END

#endif
