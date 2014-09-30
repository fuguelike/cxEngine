//
//  cxShaderMultiple.h
//  cxEngine
//  multiple texture
//  Created by xuhua on 9/18/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxShaderMultiple_h
#define cxEngine_cxShaderMultiple_h

#include <engine/cxShader.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxShaderMultiple, cxShader)
    GLint texture1;
    GLint texture2;
CX_OBJECT_END(cxShaderMultiple, cxShader)

cxString cxShaderMultipleVertex(cxAny this);

cxString cxShaderMultipleFragment(cxAny this);

CX_C_END

#endif
