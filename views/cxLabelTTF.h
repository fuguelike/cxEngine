//
//  cxLabelTTF.h
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLabelTTF_h
#define cxEngine_cxLabelTTF_h

#include "cxSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxLabelTTF, cxSprite)
    cxString text;
    cxString font;
    cxBool isDirty;
    cxTextAttr attr;
CX_OBJECT_END(cxLabelTTF, cxSprite)

void cxLabelTTFSetFontSize(cxAny pview,cxFloat size);

void cxLabelTTFSetAlign(cxAny pview,cxTextAlign align);

void cxLabelTTFSetText(cxAny pview,cxString txt);

void cxLabelTTFSetFontName(cxAny pview,cxString font);

void cxLabelTTFUpdateText(cxAny pview);

cxLabelTTF cxLabelTTFCreate(cxString txt,cxString font,cxFloat fontsize);

CX_C_END

#endif
