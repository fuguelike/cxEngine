//
//  cxWindow.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxWindow.h"

CX_OBJECT_INIT(cxWindow, cxView)
{
    this->super.parentView = NULL;
}
CX_OBJECT_FREE(cxWindow, cxView)
{

}
CX_OBJECT_TERM(cxWindow, cxView)
