//
//  cxAtlasSet.c
//  cxEngine
//
//  Created by xuhua on 1/3/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxAtlasSet.h"

void cxAtlasSetLayout(cxEvent *event)
{
    cxAtlasSet this = event->sender;
    cxAtlasClean(this);
    cxList subViews = cxViewSubViews(this);
    cxAtlasSetCapacity(this, cxListLength(subViews));
    CX_LIST_FOREACH_SAFE(subViews, ele, tmp){
        cxSprite sp = ele->any;
        if(!cxViewSupportAtlasSet(sp)){
            continue;
        }
        cxViewSetVisible(sp, false);
        CX_ASSERT(cxObjectIsType(sp, cxSpriteTypeName), "cxAtlasSet must is cxSprite subview");
        cxVec2f pos = cxViewPosition(sp);
        cxSize2f size = cxViewSize(sp);
        cxBoxTex2f tex = sp->texCoord;
        cxColor4f color = cxViewColor(sp);
        cxAtlasAppendBoxPoint(this, pos, size, tex, color);
    }
}


CX_OBJECT_INIT(cxAtlasSet, cxAtlas)
{
    CX_EVENT_QUICK(this->super.super.super.onLayout, cxAtlasSetLayout);
}
CX_OBJECT_FREE(cxAtlasSet, cxAtlas)
{
    
}
CX_OBJECT_TERM(cxAtlasSet, cxAtlas)