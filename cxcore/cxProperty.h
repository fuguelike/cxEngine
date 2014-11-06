//
//  cxProperty.h
//  cxCore
//
//  Created by xuhua on 5/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxProperty_h
#define cxCore_cxProperty_h

#include "cxCore.h"

CX_C_BEGIN

typedef void (*cxPropertySetter)(cxAny,cxAny);

typedef cxAny (*cxPropertyGetter)(cxAny);

#define CX_SETTER_DEF(_t_,_p_)  static void __##_t_##_p_##Setter(_t_ this,cxAny value)

#define CX_GETTER_DEF(_t_,_p_)  static cxAny __##_t_##_p_##Getter(_t_ this)

#define CX_SETTER(_t_,_p_)      cxTypeSetProperty(this,#_p_)->setter=__##_t_##_p_##Setter

#define CX_GETTER(_t_,_p_)      cxTypeSetProperty(this,#_p_)->getter=__##_t_##_p_##Getter

CX_DEF(cxProperty, cxObject)
    cxAny setter;
    cxAny getter;
CX_END(cxProperty, cxObject)

cxBool cxPropertyRunSetter(cxAny object,cxConstChars key,cxAny value);

cxBool cxPropertyRunGetter(cxAny object,cxConstChars key,cxAny *value);

CX_C_END

#endif
