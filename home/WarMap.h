//
//  WarMap.h
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_WarMap_h
#define Home_WarMap_h

#include <engine/cxView.h>

CX_C_BEGIN

CX_OBJECT_DEF(WarMap, cxView)
CX_OBJECT_END(WarMap, cxView)

//获取一块的显示位置
cxVec2f WarMapUnitPosition(cxAny pmap,cxVec2i idx);

CX_C_END

#endif
