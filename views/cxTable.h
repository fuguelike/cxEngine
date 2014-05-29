//
//  cxTable.h
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTable_h
#define cxEngine_cxTable_h

#include <engine/cxView.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTable, cxView)
    cxVec2f space;
    cxBool isArray;
    cxBool arrayHide;
    cxVec2i grid;
CX_OBJECT_END(cxTable)

void cxTableArrayHide(cxAny pview,cxBool arrayHide);

void cxTableArraySubviews(cxAny pview);

void cxTableSetSpace(cxAny pview,cxVec2f space);

cxTable cxTableCreate(cxVec2i grid);

CX_C_END

#endif
