//
//  cxFollow.h
//  cxEngine
//
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFollow_h
#define cxEngine_cxFollow_h

#include <core/cxAction.h>

CX_C_BEGIN

typedef cxBool (cxFollowExitFunc)(cxAny pav);

CX_OBJECT_DEF(cxFollow, cxAction)
    cxAny target;   //跟踪目标 cxNumber or cxView
    cxFloat angle;  //当前运动角度
    cxFloat speed;  //当前速度
    cxFloat init;   //开始速度
    cxFloat min;    //最小距离
    //是否结束跟踪
    CX_METHOD_ALLOC(cxBool, Exit, cxAny);
    //速度公式
    CX_METHOD_ALLOC(cxFloat, Speed,cxAny,cxFloat);
CX_OBJECT_END(cxFollow)

void cxFollowInit(cxAny pav);

void cxFollowSetInitSpeed(cxAny pav,cxFloat speed);

cxBool cxFollowDefaultExit(cxAny pav);

void cxFollowSetVec2f(cxAny pav,cxVec2f pt);

void cxFollowSetTarget(cxAny pav,cxAny target);

cxFollow cxFollowCreate(cxFloat initSpeed,cxAny target);

cxAny cxFollowTarget(cxAny pav);

CX_C_END

#endif
