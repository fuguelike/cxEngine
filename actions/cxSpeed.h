//
//  cxSpeed.h
//  engine
//
//  Created by xuhua on 9/4/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSpeed_h
#define cxEngine_cxSpeed_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxSpeed, cxAction)
    cxVec2f speed;      //begin speed
    cxVec2f accele;       //acceleration speed
    cxFloat angle;      //move angle
    cxFloat time;       //cost time
    CX_METHOD_DEF(cxBool, UseValue,cxAny,cxVec2f);
CX_OBJECT_END(cxSpeed, cxAction)

cxSpeed cxSpeedCreate(cxVec2f speed,cxVec2f accele);

CX_C_END

#endif
