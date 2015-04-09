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

CX_DEF(cxMultiple, cxAction)
    CX_FIELD_DEF(cxMultipleType Type);
    cxArray items;
    cxInt index;
CX_END(cxMultiple, cxAction)

CX_FIELD_SET(cxMultiple, cxMultipleType, Type);

void cxMultipleSetType(cxAny pav,cxMultipleType type);

void cxMultipleAppend(cxAny pav,cxAny action);

//last must is NULL
//cxMultipleCreate(cxMultipleTypeConcurrent,p1,p2,p3,NULL)
cxMultiple cxMultipleCreate(cxMultipleType type,...);

CX_C_END

#endif
