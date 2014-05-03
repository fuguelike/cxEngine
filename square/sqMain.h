//
//  sqMain.h
//  cxEngine
//
//  Created by xuhua on 4/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef square_sqMain_h
#define square_sqMain_h

#include "sqSprite.h"
#include "sqDrop.h"
#include "sqMenu.h"
#include "sqTimer.h"

CX_C_BEGIN

CX_OBJECT_DEF(sqMain, cxSprite)
    cxSize2f unitSize;
    sqDrop drop;
    sqMenu menu;
    sqTimer timer;
    sqSprite current;
CX_OBJECT_END(sqMain)

CX_C_END

#endif
