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

CX_DEF(cxLabel, cxSprite)
    cxStr text;
    cxStr font;
    cxBool isDirty;
    cxTextAttr attr;
CX_END(cxLabel, cxSprite)

void cxLabelSetStroke(cxAny pview,cxColor4f color,cxFloat witdh,cxVec2f offset);

void cxLabelSetFont(cxAny pview,cxStr font, cxColor4f color, cxFloat size);

void cxLabelSetAlign(cxAny pview,cxTextAlign align);

void cxLabelSetText(cxAny pview,cxStr txt);

void cxLabelSetBold(cxAny pview,cxBool bold);

void cxLabelUpdateText(cxAny pview);

cxLabel cxLabelCreate(cxStr txt,cxStr font,cxFloat fontsize);

CX_C_END

#endif
