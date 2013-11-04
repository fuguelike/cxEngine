//
//  cxFreeType.h
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFreeType_h
#define cxEngine_cxFreeType_h

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include "cxBase.h"
#include "cxHash.h"
#include "cxString.h"

CX_C_BEGIN

typedef union{
    int32_t key;
    struct{
        uint16_t wchar;
        uint16_t wfont;
    };
}cxFreeFontCharKey;

#define cxFontChar(c,w) (cxFreeFontCharKey){.wchar=c,.wfont=w}

CX_OBJECT_DEF(cxFreeFontChar, cxObject)
    cxFreeFontCharKey key;
    cxInt top;
    cxInt left;
    cxInt height;
    cxInt width;
    cxChar *data;
    cxInt ax;
    cxInt ay;
CX_OBJECT_END(cxFreeFontChar)

CX_OBJECT_DEF(cxFreeFont, cxObject)
    FT_Face face;
    cxString data;
    cxHash chars;
CX_OBJECT_END(cxFreeFont)

void cxFreeFontSetSize(cxFreeFont this,cxInt width,cxInt height);

cxFreeFontChar cxFreeFontLoadChar(cxFreeFont this,cxFreeFontCharKey key);

CX_OBJECT_DEF(cxFreeType, cxObject)
    cxHash fonts;
    FT_Library library;
    FT_Error error;
    CX_SLOT_ALLOC(onMemory);
CX_OBJECT_END(cxFreeType)

cxFreeType cxFreeTypeInstance();

void cxFreeTypeDestroy();

cxFreeFont cxFreeTypeCreateFont(cxConstChars file);

CX_C_END

#endif
