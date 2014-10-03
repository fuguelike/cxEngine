//
//  WarMap.c
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <chipmunk/chipmunk.h>
#include <engine/cxEngine.h>
#include <views/cxSprite.h>
#include "Define.h"
#include "WarMap.h"

CX_OBJECT_TYPE(WarMapUnit, cxSprite)
{
    
}
CX_OBJECT_INIT(WarMapUnit, cxSprite)
{
    
}
CX_OBJECT_FREE(WarMapUnit, cxSprite)
{
    
}
CX_OBJECT_TERM(WarMapUnit, cxSprite)

static void WarMapOnDirty(cxAny sender)
{
    CX_ASSERT_THIS(sender, WarMap);
    cxVec2f pos;
    if(cxViewHitTest(this, cxVec2fv(0, 0), &pos)){
        CX_LOGGER("%f %f",pos.x,pos.y);
    }
}

static cpBB UnitIndexBB(cxAny pview)
{
    CX_ASSERT_THIS(pview, WarMapUnit);
    cxVec2f pos = cxViewGetPosition(this);
    cxSize2f size = cxViewGetSize(this);
    cxFloat w = size.w / 2.0f;
    cxFloat h = size.h / 2.0f;
    return cpBBNew(pos.x - w, pos.y - h, pos.x + w, pos.y + h);
}

CX_OBJECT_TYPE(WarMap, cxView)
{
    
}
CX_OBJECT_INIT(WarMap, cxView)
{
    cxInt cells = global.warUnitNum.x * global.warUnitNum.y * 2;
    this->units = cxSpatialAlloc(global.warUnitSize.w, cells, UnitIndexBB);
    
    cxViewSetSize(this, global.warMapSize);
    CX_ADD(cxView, this, onDirty, WarMapOnDirty);
    
    for(cxInt x=0; x < global.warUnitNum.x;x++){
        for (cxInt y=0; y < global.warUnitNum.y; y++) {
            WarMapUnit u = CX_CREATE(WarMapUnit);
            cxSpriteSetTextureURL(u, "bg1.png");
            WarMapAppendUnit(this, cxVec2iv(x, y), u);
        }
    }
}
CX_OBJECT_FREE(WarMap, cxView)
{
    CX_RELEASE(this->units);
}
CX_OBJECT_TERM(WarMap, cxView)

void WarMapAppendUnit(cxAny pmap,cxVec2i idx,cxAny punit)
{
    CX_ASSERT_THIS(pmap, WarMap);
    CX_ASSERT_VALUE(punit, WarMapUnit, unit);
    WarMapUnitSetIndex(unit, idx);
    cxVec2f pos = WarMapUnitPosition(this, idx);
    cxViewSetPosition(unit, pos);
    cxViewSetSize(unit, global.warUnitSize);
    //insert and append
    cxViewAppend(this, unit);
    cxSpatialInsert(this->units, unit);
}

cxVec2f WarMapUnitPosition(cxAny pmap,cxVec2i idx)
{
    CX_ASSERT_THIS(pmap, WarMap);
    cxSize2f size = cxViewGetSize(this);
    cxVec2f pos = cxVec2fv(global.warUnitSize.w * idx.x, global.warUnitSize.h * idx.y);
    pos.x -= (size.w - global.warUnitSize.w)/2.0f;
    pos.y -= (size.h - global.warUnitSize.h)/2.0f;
    return pos;
}