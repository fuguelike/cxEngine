//
//  fcDecoration.h
//  cxEngine
//  Created by xuhua on 4/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcDecorationBegin_h
#define fruit_fcDecorationBegin_h

#include <fcDecoration.h>

CX_C_BEGIN

CX_OBJECT_DEF(fcDecorationBegin, fcDecoration)
    //当持续按下时
    cxFloat freq;   //初始化之前可设置 
    cxTimer timer;
    cxInt count;
CX_OBJECT_END(fcDecorationBegin)

void fcDecorationBeginInit(cxAny pview,cxAny map);

CX_C_END

#endif
