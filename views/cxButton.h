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
    cxVec2f touchPos;
    cxBool isEnable;
    cxBool isDown;
    CX_EVENT_ALLOC(onPress);
    CX_EVENT_ALLOC(onMove);
    CX_EVENT_ALLOC(onRelease);
    CX_EVENT_ALLOC(onLeave);
CX_OBJECT_END(cxButton)

cxBool cxButtonTouch(cxAny pview,const cxTouch *touch);

void cxButtonEnable(cxAny pview,cxBool enable);

void cxButtonXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader);

CX_C_END

#endif
