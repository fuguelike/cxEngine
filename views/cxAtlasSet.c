//
//  cxAtlasSet.c
//  cxEngine
//
//  Created by xuhua on 1/3/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxViewRoot.h>
#include "cxAtlasSet.h"

static void cxAtlasSetUpdate(cxEvent *event)
{
    cxAtlasSet this = event->sender;
    cxAtlasSetNumber(this, cxViewSubviewCount(this));
    CX_LIST_FOREACH_SAFE(this->super.super.super.subViews, ele, tmp){
        cxSprite sp = ele->any;
        CX_ASSERT(cxObjectIsType(sp, cxSpriteTypeName), "cxAtlasSet must is cxSprite subview");
        CX_ASSERT(sp->texture == this->super.super.texture, "must is same texture");
        cxVec2f pos = cxViewPosition(sp);
        cxSize2f size = cxViewSize(sp);
        cxBoxTex2f tex = sp->texCoord;
        cxColor4f color = cxViewColor(sp);
        cxAtlasAppendBoxPoint(this, pos, size, tex, color);
        cxViewRemoved(sp);
    }
}

void cxAtlasSetReadAttr(cxReaderAttrInfo *info)
{
    cxAtlasReadAttr(info);
}

CX_OBJECT_INIT(cxAtlasSet, cxAtlas)
{
    CX_EVENT_QUICK(this->super.super.super.onUpdate, cxAtlasSetUpdate);
    cxObjectSetReadAttrFunc(this, cxAtlasSetReadAttr);
}
CX_OBJECT_FREE(cxAtlasSet, cxAtlas)
{
    
}
CX_OBJECT_TERM(cxAtlasSet, cxAtlas)