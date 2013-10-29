//
//  cxTable.h
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTable_h
#define cxEngine_cxTable_h

#include <core/cxView.h>

CX_C_BEGIN

typedef enum {
    cxTableArrayNone,
    cxTableArrayVertical,   //vertical
    cxTableArrayHorizon,    //horizon
    cxTableArrayGrid,
}cxTableArrayType;

CX_OBJECT_DEF(cxTable, cxView)
    cxTableArrayType type;
    cxVec2f space;
    cxBool isArray;
    cxVec2i grid;
CX_OBJECT_END(cxTable)

cxBool cxTableXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader);

void cxTableArraySubViews(cxAny pview);

void cxTableSetType(cxAny pview,cxTableArrayType type);

void cxTableSetSpace(cxAny pview,cxVec2f space);

CX_C_END

#endif
