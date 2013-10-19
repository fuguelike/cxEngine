//
//  cxCurve.h
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxCurve_h
#define cxEngine_cxCurve_h

#include <core/cxBase.h>
#include <core/cxHash.h>

CX_C_BEGIN

typedef cxFloat (*cxActionCurveFunc)(cxAny pav,cxFloat time);

CX_OBJECT_DEF(cxCurveItem, cxObject)
    cxActionCurveFunc func;
CX_OBJECT_END(cxCurveItem)

CX_OBJECT_DEF(cxCurve, cxObject)
    cxHash curves;
CX_OBJECT_END(cxCurve)

void cxCurveRegister(cxCurve this,cxConstChars name,cxActionCurveFunc func);

cxFloat cxCurveCubeIn(void *pav,cxFloat time);

cxFloat cxCurveCubeOut(void *pav,cxFloat time);

cxFloat cxCurveEaseIn(void *pav,cxFloat time);

cxFloat cxCurveEaseOut(void *pav,cxFloat time);

cxFloat cxCurveCubeInverse(void *pav,cxFloat time);

CX_C_END

#endif
