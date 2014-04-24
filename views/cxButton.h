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

CX_OBJECT_DEF(cxButton, cxSprite)
    cxBool isEnable;
    cxBool isDown;
    cxFloat movement;
    CX_EVENT_ALLOC(onPress);
    CX_EVENT_ALLOC(onRelease);
    CX_EVENT_ALLOC(onEnter);
    CX_EVENT_ALLOC(onLeave);
CX_OBJECT_END(cxButton)

cxBool cxButtonTouch(cxAny pview,cxTouch *touch);

void cxButtonEnable(cxAny pview,cxBool enable);

cxButton cxButtonCreate(cxConstChars url);

CX_C_END

#endif
