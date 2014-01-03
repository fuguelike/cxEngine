//
//  cxAtlasSet.h
//  cxEngine
//
//  Created by xuhua on 1/3/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAtlasSet_h
#define cxEngine_cxAtlasSet_h

#include "cxAtlas.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxAtlasSet, cxAtlas)

CX_OBJECT_END(cxAtlasSet)

void cxAtlasSetLayout(cxEvent *event);

void cxAtlasSetReadAttr(cxReaderAttrInfo *info);

CX_C_END

#endif
