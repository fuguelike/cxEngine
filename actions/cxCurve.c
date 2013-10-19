//
//  cxCurve.c
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxCurve.h"

CX_OBJECT_INIT(cxCurveItem, cxObject)
{
    
}
CX_OBJECT_FREE(cxCurveItem, cxObject)
{
    
}
CX_OBJECT_TERM(cxCurveItem, cxObject)

cxFloat cxCurveEaseIn(void *pav,cxFloat time)
{
    return powf(time, 2.0f);
}

cxFloat cxCurveEaseOut(void *pav,cxFloat time)
{
    return powf(time, 1.0f / 2.0f);
}

cxFloat cxCurveCubeIn(void *pav,cxFloat time)
{
    return powf(time, 3.0f);
}

cxFloat cxCurveCubeOut(void *pav,cxFloat time)
{
    return powf(time, 1.0f / 3.0f);
}

cxFloat cxCurveCubeInverse(void *pav,cxFloat time)
{
    time = time - 1.0f;
    return (powf(time, 3.0f) + 1.0f);
}

void cxCurveRegister(cxCurve this,cxConstChars name,cxActionCurveFunc func)
{
    cxCurveItem item = CX_ALLOC(cxCurveItem);
    item->func = func;
    cxHashSet(this->curves, cxHashStrKey(name), item);
    CX_RELEASE(item);
}

CX_OBJECT_INIT(cxCurve, cxObject)
{
    this->curves = CX_ALLOC(cxHash);
    cxCurveRegister(this,"cxCurveEaseIn",cxCurveEaseIn);
    cxCurveRegister(this,"cxCurveEaseOut",cxCurveEaseOut);
    cxCurveRegister(this,"cxCurveCubeInverse",cxCurveCubeInverse);
    cxCurveRegister(this,"cxCurveCubeIn",cxCurveCubeIn);
    cxCurveRegister(this,"cxCurveCubeOut",cxCurveCubeOut);
}
CX_OBJECT_FREE(cxCurve, cxObject)
{
    CX_RELEASE(this->curves);
}
CX_OBJECT_TERM(cxCurve, cxObject)

