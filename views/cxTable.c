//
//  cxTable.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTable.h"
#include "cxItem.h"

CX_OBJECT_INIT(cxTable, cxView)
{
    this->super.isCropping = true;
}
CX_OBJECT_FREE(cxTable, cxView)
{
    
}
CX_OBJECT_TERM(cxTable, cxView)
