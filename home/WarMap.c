//
//  WarMap.c
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <views/cxSprite.h>
#include "Define.h"
#include "WarMap.h"

CX_OBJECT_TYPE(WarMap, cxView)
{
    
}
CX_OBJECT_INIT(WarMap, cxView)
{
    cxViewSetSize(this, global.warMapSize);
    
    for(cxInt x=0; x < global.warUnitNum.x;x++){
        for (cxInt y=0; y < global.warUnitNum.y; y++) {
            cxVec2f pos = WarMapUnitPosition(this, cxVec2iv(x, y));
            cxSprite sp = cxSpriteCreateWithURL("bg1.png");
            cxViewSetSize(sp, global.warUnitSize);
            cxViewSetPosition(sp, pos);
            cxViewAppend(this, sp);
        }
    }
}
CX_OBJECT_FREE(WarMap, cxView)
{
    
}
CX_OBJECT_TERM(WarMap, cxView)

cxVec2f WarMapUnitPosition(cxAny pmap,cxVec2i idx)
{
    CX_ASSERT_THIS(pmap, WarMap);
    cxSize2f size = cxViewGetSize(this);
    cxVec2f pos = cxVec2fv(global.warUnitSize.w * idx.x, global.warUnitSize.h * idx.y);
    pos.x -= (size.w - global.warUnitSize.w)/2.0f;
    pos.y -= (size.h - global.warUnitSize.h)/2.0f;
    return pos;
}