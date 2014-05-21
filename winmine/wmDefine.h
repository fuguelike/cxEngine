//
//  wmDefine.h
//  winmine
//
//  Created by xuhua on 14-5-20.
//  Copyright (c) 2014年 xuhua. All rights reserved.
//

#ifndef winmine_wmDefine_h
#define winmine_wmDefine_h

#include <engine/cxEngine.h>

CX_C_BEGIN

typedef enum {
    wmDirNone           = 0,
    wmDirLeft           = 1 << 0,
    wmDirRight          = 1 << 1,
    wmDirTop            = 1 << 2,
    wmDirBottom         = 1 << 3,
    wmDirLeftTop        = wmDirLeft | wmDirTop,
    wmDirLeftBottom     = wmDirLeft | wmDirBottom,
    wmDirRightTop       = wmDirRight | wmDirTop,
    wmDirRightBottom    = wmDirRight | wmDirBottom,
}wmDir;

CX_C_END

#endif
