//
//  WaterShader.h
//  cxEngineStatic
//
//  Created by xuhua on 3/2/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngine_WaterShader_h
#define cxEngine_WaterShader_h

#include <cxcore/cxBase.h>
#include <engine/cxShader.h>

CX_C_BEGIN

CX_DEF(WaterShader, cxShader)
    GLuint center;
    GLuint resolution;
    GLuint time;
CX_END(WaterShader, cxShader)

CX_C_END

#endif
