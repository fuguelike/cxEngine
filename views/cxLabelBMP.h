//
//  cxLabelBMP.h
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLabelBMP_h
#define cxEngine_cxLabelBMP_h

#include <core/cxBMPFont.h>
#include "cxAtlas.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxLabelBMP, cxAtlas)
    cxBMPFont font;
    cxString txt;
    cxBool isDirty;
    cxFloat size;
CX_OBJECT_END(cxLabelBMP)

void cxLabelBMPInitView(cxAny pview,cxJson json);

cxLabelBMP cxLabelBMPCreate(cxString font,cxString txt);

void cxLabelBMPSetFont(cxAny pview,cxString font);

void cxLabelBMPSetText(cxAny pview,cxString txt);

CX_C_END

#endif
