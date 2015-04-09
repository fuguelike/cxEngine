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

CX_DEF(cxTable, cxView)
    CX_FIELD_DEF(cxVec2i Grid);
CX_END(cxTable, cxView)

CX_FIELD_GET(cxTable, cxVec2i, Grid);
CX_INLINE void cxTableSetGrid(cxAny pthis,const cxVec2i value)
{
    CX_ASSERT_THIS(pthis, cxTable);
    CX_RETURN(cxVec2iEqu(this->Grid, value));
    this->Grid = value;
    cxViewSetDirty(this, cxViewDirtyForce);
}

void cxTableArraySubviews(cxAny pview);

CX_INLINE void cxTableForceArray(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxTable);
    cxTableArraySubviews(this);
}

cxTable cxTableCreate(cxVec2i grid);

CX_C_END

#endif
