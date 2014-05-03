//
//  sqDrop.h
//  cxEngine
//
//  Created by xuhua on 4/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef square_sqDrop_h
#define square_sqDrop_h

#include <core/cxView.h>

CX_C_BEGIN

CX_OBJECT_DEF(sqDrop, cxView)
    cxAny main;
CX_OBJECT_END(sqDrop)

cxAny sqDropAppendItem(sqDrop this,cxVec2i idx);

CX_C_END

#endif
