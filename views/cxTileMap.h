//
//  cxTileMap.h
//  cxEngine
//
//  Created by xuhua on 8/11/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTileMap_h
#define cxEngine_cxTileMap_h

#include <engine/cxView.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTileMap, cxView)
    cxSize2i size;
    cxInt *tags;
CX_OBJECT_END(cxTileMap, cxView)

void cxTileMapAppend(cxAny pview,cxAny newview);

cxTileMap cxTileMapCreate(cxSize2i size);

CX_C_END

#endif
