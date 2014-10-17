//
//  cxCurve.h
//  cxEngine
//  曲线
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxCurve_h
#define cxEngine_cxCurve_h

#include <cxCore/cxBase.h>

CX_C_BEGIN

typedef cxFloat (*cxActionCurveFunc)(cxAny pav,cxFloat time);

CX_DEF(cxCurveItem, cxObject)
    cxActionCurveFunc func;
CX_END(cxCurveItem, cxObject)

CX_DEF(cxCurve, cxObject)
    cxHash curves;
CX_END(cxCurve, cxObject)

cxCurveItem cxCurveGet(cxConstChars key);

cxFloat cxCurveBackIn(cxAny pav,cxFloat time);

cxFloat cxCurveBackOut(cxAny pav,cxFloat time);

cxFloat cxCurveBackInOut(cxAny pav,cxFloat time);

cxFloat cxCurveBounceIn(cxAny pav,cxFloat time);

cxFloat cxCurveBounceOut(cxAny pav,cxFloat time);

cxFloat cxCurveBounceInOut(cxAny pav,cxFloat time);

cxFloat cxCurveElasticIn(cxAny pav,cxFloat time);

cxFloat cxCurveElasticOut(cxAny pav,cxFloat time);

cxFloat cxCurveElasticInOut(cxAny pav,cxFloat time);

cxFloat cxCurveSineIn(cxAny pav,cxFloat time);

cxFloat cxCurveSineOut(cxAny pav,cxFloat time);

cxFloat cxCurveSineInOut(cxAny pav,cxFloat time);

cxFloat cxCurveCubeIn(cxAny pav,cxFloat time);

cxFloat cxCurveCubeInOut(cxAny pav,cxFloat time);

cxFloat cxCurveCubeOut(cxAny pav,cxFloat time);

cxFloat cxCurveEaseIn(cxAny pav,cxFloat time);

cxFloat cxCurveEaseInOut(cxAny pav,cxFloat time);

cxFloat cxCurveEaseOut(cxAny pav,cxFloat time);

cxFloat cxCurveCubeInverse(cxAny pav,cxFloat time);

cxFloat cxCurveExpIn(cxAny pav,cxFloat time);

cxFloat cxCurveExpOut(cxAny pav,cxFloat time);

cxFloat cxCurveExpInOut(cxAny pav,cxFloat time);
CX_C_END

#endif
