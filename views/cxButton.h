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
    cxBool isSelected;
    cxFloat movement;           //when touch move > movement will fire onLeave,but don't fire onRelease
    CX_EVENT_ALLOC(onPress);
    CX_EVENT_ALLOC(onRelease);
    CX_EVENT_ALLOC(onEnter);
    CX_EVENT_ALLOC(onLeave);
CX_OBJECT_END(cxButton, cxSprite)

cxBool cxButtonTouch(cxAny pview,cxTouchItems *points);

void cxButtonEnable(cxAny pview,cxBool enable);

cxButton cxButtonCreate(cxConstChars url);

CX_C_END

#endif
