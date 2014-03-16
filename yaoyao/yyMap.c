//
//  yyMap.c
//  yaoyao
//
//  Created by xuhua on 3/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include <core/cxEngine.h>
#include "yyMap.h"

/*  坐标系
    9
    8
    7
    6
    5
    4
    3
    2
    1
 x  0 1 2 3 4 5 6 7 8 9
 */

cxVec2f yyMapGetPos(yyMap this,cxVec2i idx)
{
    cxSize2f mapSize = cxViewSize(this);
    cxVec2f v;
    v.x = idx.x * this->itemSize.w - (mapSize.w - this->itemSize.w)/2.0f;
    v.y = idx.y * this->itemSize.h - (mapSize.h - this->itemSize.h)/2.0f;
    return v;
}

cxVec2i yyMapGetIdx(yyMap this,cxVec2f pos)
{
    cxSize2f mapSize = cxViewSize(this);
    cxVec2i v;
    v.x = (pos.x + (mapSize.w - this->itemSize.w)/2.0f) / this->itemSize.w;
    v.y = (pos.y + (mapSize.h - this->itemSize.h)/2.0f) / this->itemSize.h;
    return v;
}

void yyMapAppendUnit(yyMap this,cxVec2i idx,cxUInt value)
{
    cxVec2f pos = yyMapGetPos(this, idx);
    cxBoxTex2f box = cxTextureBox(this->super.super.texture, value < 128 ?"map-disable.png":"map-enable.png");
    cxAtlasAppendBoxPoint(this, pos, this->itemSize, box , cxColor4fv(1, 1, 1, 1));
    this->items[idx.x][idx.y] = value;
}

CX_OBJECT_INIT(yyMap, cxAtlas)
{
    cxAtlasSetNumber(this, YY_MAP_ROW * YY_MAP_COL);
    cxTexture texture = cxTextureCreate("map.xml");
    CX_ASSERT(texture != NULL, "get map.xml texture falied");
    cxSpriteSetTexture(this, texture);
    
    cxSize2f size = cxEngineInstance()->winsize;
    cxFloat w = size.w - 10;
    cxFloat h = w;
    cxViewSetSize(this, cxSize2fv(w, h));
    this->itemSize = cxSize2fv(w/YY_MAP_ROW, h/YY_MAP_COL);
    
    //random map
    for(cxInt i=0; i < 10; i++){
        for(cxInt j=0; j < 10; j++){
            yyMapAppendUnit(this, cxVec2iv(i, j), cxRand(0, 256));
        }
    }
}
CX_OBJECT_FREE(yyMap, cxAtlas)
{

}
CX_OBJECT_TERM(yyMap, cxAtlas)









