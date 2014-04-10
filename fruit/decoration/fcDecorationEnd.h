//
//  fcDecorationEnd.h
//  cxEngine
//
//  Created by xuhua on 4/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcDecorationEnd_h
#define fruit_fcDecorationEnd_h

#include <fcDecoration.h>

CX_C_BEGIN

CX_OBJECT_DEF(fcDecorationEnd, fcDecoration)

CX_OBJECT_END(fcDecorationEnd)

void fcDecorationEndOnArrive(cxAny pview,cxAny sprite);

void fcDecorationEndInit(cxAny pview,cxAny map);

CX_C_END

#endif
