//
//  cxLabel.h
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLabel_h
#define cxEngine_cxLabel_h

#include "cxSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxLabel, cxSprite)
    cxString text;
    cxString font;
    cxBool isDirty;
    cxTextAttr attr;
CX_OBJECT_END(cxLabel)

cxBool cxLabelXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader);

void cxLabelSetFontSize(cxAny pview,cxFloat fontsize);

void cxLabelSetText(cxAny pview,cxString txt);

void cxLabelSetFont(cxAny pview,cxString font);

void cxLabelUpdateText(cxAny pview);

cxLabel cxLabelCreate(cxString txt,cxString font,cxFloat fontsize);

CX_C_END

#endif
