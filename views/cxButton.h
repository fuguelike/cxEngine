//
//  cxButton.h
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxButton_h
#define cxEngine_cxButton_h

#include <views/cxSprite.h>

CX_C_BEGIN

CX_DEF(cxButton, cxSprite)
    cxBool isSelected;
    CX_FIELD_DEF(cxBool IsEnable);
    CX_FIELD_DEF(cxBool IsPass);
    CX_EVENT_ALLOC(onTap);
CX_END(cxButton, cxSprite)

CX_FIELD_IMP(cxButton, cxBool, IsEnable);
CX_FIELD_IMP(cxButton, cxBool, IsPass);

void cxButtonPass(cxAny pview,cxBool pass);

CX_C_END

#endif
