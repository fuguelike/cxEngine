//
//  cxShaderDefault.h
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxShaderDefault_h
#define cxEngine_cxShaderDefault_h

#include <engine/cxShader.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxShaderDefault, cxShader)
    
CX_OBJECT_END(cxShaderDefault, cxShader)

cxString cxShaderDefaultGetVertexSource(cxAny this);

cxString cxShaderDefaultGetFragmentSource(cxAny this);

CX_C_END

#endif
