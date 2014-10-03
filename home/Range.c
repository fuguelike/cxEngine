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
    cxSpriteSetTextureURL(this, "bullet.json");
}
CX_OBJECT_FREE(Range, cxAtlas)
{

}
CX_OBJECT_TERM(Range, cxAtlas)

void RangeUpdateValue(cxAny pview)
{
    CX_ASSERT_THIS(pview, Range);
    cxAtlasClear(this);
    cxTexture texture = cxSpriteGetTexture(this);
    if(this->range.max > 0){

        cxFloat d = global.sideLen * this->range.max * 2.75f;
        cxFloat h = cosf(global.angle) * d;
        cxFloat w = sinf(global.angle) * d;
        
        cxViewSetSize(this, cxSize2fv(w, h));
        
        cxBoxTex2f tex = cxTextureBoxPixel(texture, "range.png",1.0f, false,false);
        cxColor4f color = cxColor4fv(1, 1, 1, 1);
        cxSize2f size = cxSize2fv(w/2.0f,h/2.0f);
        
        cxVec2f pos = cxVec2fv(w/4, h/4);
        cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex1 = cxTextureBoxPixel(texture, "range.png",1.0f, true,false);
        pos = cxVec2fv(-w/4, h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex1, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex2 = cxTextureBoxPixel(texture, "range.png",1.0f, true,true);
        pos = cxVec2fv(-w/4, -h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex2, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex3 = cxBoxTex2fFlip(tex, false, true);
        pos = cxVec2fv(w/4, -h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex3, color);
        cxAtlasAppend(this, &bp);
        
    }
    if(this->range.min > 0){
        cxFloat d = global.sideLen * this->range.min * 2.75f;
        cxFloat h = cosf(global.angle) * d;
        cxFloat w = sinf(global.angle) * d;
        
        cxBoxTex2f tex = cxTextureBoxPixel(texture, "range.png",1.0f,false,false);
        cxColor4f color = cxColor4fv(1, 0, 0, 1);
        cxSize2f size = cxSize2fv(w/2.0f,h/2.0f);
        
        cxVec2f pos = cxVec2fv(w/4, h/4);
        cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex1 = cxTextureBoxPixel(texture, "range.png",1.0f, true,false);
        pos = cxVec2fv(-w/4, h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex1, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex2 = cxTextureBoxPixel(texture, "range.png",1.0f, true,true);
        pos = cxVec2fv(-w/4, -h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex2, color);
        cxAtlasAppend(this, &bp);
        
        cxBoxTex2f tex3 = cxTextureBoxPixel(texture, "range.png",1.0f, false,true);
        pos = cxVec2fv(w/4, -h/4);
        bp = cxAtlasCreateBoxPoint(pos, size, tex3, color);
        cxAtlasAppend(this, &bp);
    }
}

void RangeSetRange(cxAny pview,cxRange2f range)
{
    CX_ASSERT_THIS(pview, Range);
    CX_RETURN(cxRange2fEqu(this->range, range));
    this->range = range;
    RangeUpdateValue(pview);
}



