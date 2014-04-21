//
//  wBoxItem.h
//  cxEngine
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef wbox_wBoxItem_h
#define wbox_wBoxItem_h

#include <views/cxSprite.h>
#include "wBoxDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(wBoxItem, cxSprite)
    cxAny box;
CX_OBJECT_END(wBoxItem)

void wBoxItemSetColor(cxAny ite,wUnitColor color);

wBoxItem wBoxItemCreate(cxAny box,cxVec2i idx);

CX_C_END

#endif
