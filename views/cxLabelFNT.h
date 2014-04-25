//
//  cxLabelFNT.h
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLabelFNT_h
#define cxEngine_cxLabelFNT_h

#include <core/cxBMPFont.h>
#include "cxAtlas.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxLabelFNT, cxAtlas)
    cxBMPFont font;
    cxString txt;
    cxBool isDirty;
    cxFloat size;
CX_OBJECT_END(cxLabelFNT)

void cxLabelFNTSetFont(cxAny pview,cxString font);

void cxLabelFNTSetText(cxAny pview,cxString txt);

void cxLabelFNTSetSize(cxAny pview,cxFloat size);

CX_C_END

#endif
