//
//  yyMap.h
//  cxEngine
//
//  Created by xuhua on 3/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef yaoyao_yyMap_h
#define yaoyao_yyMap_h

#include <views/cxAtlas.h>

CX_C_BEGIN

#define YY_MAP_ROW  10
#define YY_MAP_COL  10

CX_OBJECT_DEF(yyMap, cxAtlas)
    cxSize2f itemSize;
    cxUInt items[YY_MAP_ROW][YY_MAP_COL];
CX_OBJECT_END(yyMap)

cxVec2f yyMapGetPos(yyMap this,cxVec2i idx);

cxVec2i yyMapGetIdx(yyMap this,cxVec2f pos);

void yyMapAppendUnit(yyMap this,cxVec2i idx,cxUInt value);

CX_C_END

#endif
