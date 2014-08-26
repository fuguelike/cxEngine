//
//  cxMultiple.h
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxMultiple_h
#define cxEngine_cxMultiple_h

#include <engine/cxAction.h>

CX_C_BEGIN

typedef enum {
    cxMultipleTypeNone,      //none
    cxMultipleTypeConcurrent,//concurrent
    cxMultipleTypeSequence,  //sequence
}cxMultipleType;

CX_OBJECT_DEF(cxMultiple, cxAction)
    cxMultipleType type;
    cxArray items;
    cxInt index;
CX_OBJECT_END(cxMultiple, cxAction)

void cxMultipleSetType(cxAny pav,cxMultipleType type);

void cxMultipleAppend(cxAny pav,cxAny action);

CX_C_END

#endif
