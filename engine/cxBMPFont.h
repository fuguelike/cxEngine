//
//  cxBMPFont.h
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxBMPFont_h
#define cxEngine_cxBMPFont_h

#include <cxcore/cxBase.h>
#include <streams/cxAssetsStream.h>
#include "cxTexture.h"


CX_C_BEGIN

//id="124" x="2" y="3" width="14" height="70" xoffset="4" yoffset="-1" xadvance="17" page="0" chnl="0" letter="|"
CX_DEF(cxBMPElement, cxObject)
    cxRect4f box;
    cxInt xoffset;
    cxInt yoffset;
    cxInt xadvance;
    cxInt page;
    cxInt key;
CX_END(cxBMPElement, cxObject)

//key = (first<<16) | (second & 0xffff);
CX_DEF(cxBMPKerning, cxObject)
    cxInt first;
    cxInt second;
    cxInt amount;
CX_END(cxBMPKerning, cxObject)

#define cxBMPKerningKey(f,s)    cxHashIntKey(((f) << 16) | ((s) & 0xffff))

CX_DEF(cxBMPFont, cxObject)
    cxStr face;
    cxInt bold;
    cxInt stretchH;
    cxInt smooth;
    cxInt aa;
    cxBox4i padding;
    cxVec2i spacing;
    cxInt italic;
    cxStr chasrset;
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
CX_END(cxBMPFont, cxObject)

cxFloat cxBMPKerningAmount(cxBMPFont this, cxUInt16 prev,cxUInt16 curr);

cxTexture cxBMPFontTexture(cxBMPFont this,cxUChar page);

cxBMPElement cxBMPFontChar(cxBMPFont this,cxUInt id);

cxBool cxBMPFontLoad(cxBMPFont this,cxConstChars file);

cxBMPFont cxBMPFontCreate(cxConstChars file);

CX_C_END

#endif
