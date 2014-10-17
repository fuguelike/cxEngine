//
//  cxButton.h
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxButton_h
#define cxEngine_cxButton_h

#include "cxSprite.h"

CX_C_BEGIN

CX_DEF(cxButton, cxSprite)
    cxVec2f touchPos;
    cxFloat movement;
    cxBool isPass;
    cxBool isEnable;
    cxBool isSelected;
    CX_EVENT_ALLOC(onPress);
    CX_EVENT_ALLOC(onRelease);
    CX_EVENT_ALLOC(onEnter);
    CX_EVENT_ALLOC(onLeave);
CX_END(cxButton, cxSprite)

cxBool cxButtonTouch(cxAny pview,const cxTouchItems *points);

void cxButtonEnable(cxAny pview,cxBool enable);

void cxButtonPass(cxAny pview,cxBool pass);

cxButton cxButtonCreate(cxConstChars url);

CX_C_END

#endif
