//
//  cxActionMgr.c
//  cxEngine
//
//  Created by xuhua on 8/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxActionMgr.h"

CX_OBJECT_TYPE(cxActionMgr, cxObject)
{
    
}
CX_OBJECT_INIT(cxActionMgr, cxObject)
{
    this->scale = 1.0f;
}
CX_OBJECT_FREE(cxActionMgr, cxObject)
{

}
CX_OBJECT_TERM(cxActionMgr, cxObject)

cxActionMgr cxActionMgrCreate()
{
    cxActionMgr this = CX_CREATE(cxActionMgr);
    return this;
}