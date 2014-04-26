//
//  cxBMPFont.h
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxBMPFont_h
#define cxEngine_cxBMPFont_h

#include <streams/cxAssetsStream.h>
#include "cxTexture.h"
#include "cxBase.h"
#include "cxArray.h"

CX_C_BEGIN

//id="124" x="2" y="3" width="14" height="70" xoffset="4" yoffset="-1" xadvance="17" page="0" chnl="0" letter="|"
CX_OBJECT_DEF(cxBMPElement, cxObject)
    cxRect4f box;
    cxInt xoffset;
    cxInt yoffset;
    cxInt xadvance;
    cxInt page;
CX_OBJECT_END(cxBMPElement)

//key = (first<<16) | (second & 0xffff);
CX_OBJECT_DEF(cxBMPKerning, cxObject)
    cxInt first;
    cxInt second;
    cxInt amount;
CX_OBJECT_END(cxBMPKerning)

#define cxBMPKerningKey(f,s)    (((f) << 16) | ((s) & 0xffff))

CX_OBJECT_DEF(cxBMPFont, cxObject)
    cxString face;
    cxInt bold;
    cxInt stretchH;
    cxInt smooth;
    cxInt aa;
    cxBox4i padding;
    cxVec2i spacing;
    cxInt italic;
    cxString chasrset;
    cxInt size;
    cxInt unicode;
    cxInt lineHeight;
    cxInt base;
    cxSize2f scale;
    cxInt pages;
    cxInt packed;
    cxHash textures;
    cxHash chars;
    cxHash kernings;
    cxBool isError;
CX_OBJECT_END(cxBMPFont)

cxFloat cxBMPKerningAmount(cxBMPFont this, cxUInt16 prev,cxUInt16 curr);

cxTexture cxBMPFontTexture(cxBMPFont this,cxUChar page);

cxBMPElement cxBMPFontChar(cxBMPFont this,cxUInt id);

cxBool cxBMPFontLoad(cxBMPFont this,cxConstChars file);

cxBMPFont cxBMPFontCreate(cxConstChars file);

CX_C_END

#endif
