//
//  fcDefine.h
//  cxEngine
//
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcDefine_h
#define fruit_fcDefine_h

#include <core/cxBase.h>

CX_C_BEGIN

#define DM_MAP_WIDTH    10

#define DM_MAP_HEIGHT   10

typedef struct {
    cxVec2i points[DM_MAP_WIDTH*DM_MAP_HEIGHT];
    cxInt number;
}fcPath;

CX_OBJECT_DEF(fcDefine, cxObject)

CX_OBJECT_END(fcDefine)

CX_C_END

#endif
