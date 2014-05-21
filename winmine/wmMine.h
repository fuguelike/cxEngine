//
//  wmMine.h
//  winmine
//
//  Created by xuhua on 14-5-20.
//  Copyright (c) 2014年 xuhua. All rights reserved.
//

#ifndef winmine_wmMine_h
#define winmine_wmMine_h

#include <views/cxSprite.h>

CX_C_BEGIN

CX_OBJECT_DEF(wmMine, cxSprite)
    cxAny map;
    cxInt value;
CX_OBJECT_END(wmMine)

CX_C_END

#endif
