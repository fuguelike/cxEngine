//
//  cxEventItem.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxEventItem.h"

CX_OBJECT_INIT(cxEventItem, cxObject)
{

}
CX_OBJECT_FREE(cxEventItem, cxObject)
{
    CX_RELEASE(this->arg);
}
CX_OBJECT_TERM(cxEventItem, cxObject)

CX_OBJECT_INIT(cxFuncItem, cxObject)
{
}
CX_OBJECT_FREE(cxFuncItem, cxObject)
{
}
CX_OBJECT_TERM(cxFuncItem, cxObject)