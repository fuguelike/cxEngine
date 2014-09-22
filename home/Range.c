//
//  Range.c
//  Home
//
//  Created by xuhua on 9/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "Define.h"
#include "Range.h"

CX_OBJECT_TYPE(Range, cxAtlas)
{
    
}
CX_OBJECT_INIT(Range, cxAtlas)
{
    cxSpriteSetTextureURL(this, "range.png");
}
CX_OBJECT_FREE(Range, cxAtlas)
{

}
CX_OBJECT_TERM(Range, cxAtlas)

void RangeUpdateValue(cxAny pview)
{
    CX_ASSERT_THIS(pview, Range);
    cxAtlasClean(this);
    if(this->value.max > 0){

        cxFloat d = global.sideLen * this->value.max * 2.75f;
        cxFloat h = cosf(global.angle) * d;
        cxFloat w = sinf(global.angle) * d;
        
        cxViewSetSize(this, cxSize2fv(w, h));
        
        cxBoxTex2f tex = (cxBoxTex2f){cxTex2fv(0.0f, 0.0f),cxTex2fv(0.0f, 1.0f),cxTex2fv(1.0f, 0.0f),cxTex2fv(1.0f, 1.0f)};
        cxColor4f color = cxColor4fv(0, 1, 0, 1);
        cxSize2f size = cxSize2fv(w/2.0f,h/2.0f);
        
        cxVec2f pos = cxVec2fv(w/4, h/4);
        cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex1 = cxBoxTex2fFlip(tex, true, false);
        pos = cxVec2fv(-w/4, h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex1, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex2 = cxBoxTex2fFlip(tex, true, true);
        pos = cxVec2fv(-w/4, -h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex2, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex3 = cxBoxTex2fFlip(tex, false, true);
        pos = cxVec2fv(w/4, -h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex3, color);
        cxAtlasAppend(this, &bp);
        
    }
    if(this->value.min > 0){
        cxFloat d = global.sideLen * this->value.min * 2.75f;
        cxFloat h = cosf(global.angle) * d;
        cxFloat w = sinf(global.angle) * d;
        
        cxBoxTex2f tex = (cxBoxTex2f){cxTex2fv(0.0f, 0.0f),cxTex2fv(0.0f, 1.0f),cxTex2fv(1.0f, 0.0f),cxTex2fv(1.0f, 1.0f)};
        cxColor4f color = cxColor4fv(1, 0, 0, 1);
        cxSize2f size = cxSize2fv(w/2.0f,h/2.0f);
        
        cxVec2f pos = cxVec2fv(w/4, h/4);
        cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex1 = cxBoxTex2fFlip(tex, true, false);
        pos = cxVec2fv(-w/4, h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex1, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex2 = cxBoxTex2fFlip(tex, true, true);
        pos = cxVec2fv(-w/4, -h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex2, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex3 = cxBoxTex2fFlip(tex, false, true);
        pos = cxVec2fv(w/4, -h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex3, color);
        cxAtlasAppend(this, &bp);
    }
}

void RangeSetMin(cxAny pview,cxFloat min)
{
    CX_ASSERT_THIS(pview, Range);
    CX_RETURN(cxFloatEqu(this->value.min, min));
    this->value.min = min;
    RangeUpdateValue(pview);
}

void RangeSetMax(cxAny pview,cxFloat max)
{
    CX_ASSERT_THIS(pview, Range);
    CX_RETURN(cxFloatEqu(this->value.max, max));
    this->value.max = max;
    RangeUpdateValue(pview);
}

