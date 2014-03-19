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
#include <core/cxJson.h>

CX_C_BEGIN

#define YY_MAP_ROW  10
#define YY_MAP_COL  10

#define YY_MAP_MOVE_VALUE 0xFF

CX_OBJECT_DEF(yyMap, cxAtlas)
    cxJson mapJson;
    cxSize2f itemSize;
    cxUInt items[YY_MAP_ROW][YY_MAP_COL];
    //
    cxBool isTouchStart;
    cxVec2i bIdx;//touch begin idx
    cxVec2i eIdx;//touch end idx
CX_OBJECT_END(yyMap)

cxBool yyCheckIdx(cxVec2i idx);

cxBool yyMapCanMove(yyMap this,cxVec2i idx);

cxVec2f yyMapGetPos(yyMap this,cxVec2i idx);

cxVec2i yyMapGetIdx(yyMap this,cxVec2f pos);

cxBool yyMapLoad(yyMap this,cxInt mapIdx);

CX_C_END

#endif
