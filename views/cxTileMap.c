//
//  cxTileMap.c
//  cxEngine
//
//  Created by xuhua on 8/11/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxTileMap.h"

/*
45度贴图宽高比例
w/h = 4/3
+++++++++++++++++++++++
+++++++++++++++++++++++
+++++++++++++++++++++++
+++++++++++++++++++++++
+++++++++++++++++++++++
*/

void cxTileMapAppend(cxAny pview,cxAny newview)
{
//    cxTileMap this = pview;
    //resize view
    
    cxViewAppendImp(pview, newview);
}

CX_OBJECT_TYPE(cxTileMap, cxView)
{
    
}
CX_OBJECT_INIT(cxTileMap, cxView)
{
    CX_METHOD_SET(this->cxView.Append, cxTileMapAppend);
}
CX_OBJECT_FREE(cxTileMap, cxView)
{
    allocator->free(this->tags);
}
CX_OBJECT_TERM(cxTileMap, cxView)

cxTileMap cxTileMapCreate(cxSize2i size)
{
    cxTileMap this = CX_CREATE(cxTileMap);
    this->size = size;
    this->tags = allocator->malloc(size.w * size.h * sizeof(cxInt));
    return this;
}