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

#define CX_SETTER_DEF(_t_,_p_)     static void __##_t_##_p_##Setter(_t_ this,cxAny value)

#define CX_GETTER_DEF(_t_,_p_)     static cxAny __##_t_##_p_##Getter(_t_ this)

#define CX_SETTER(_t_,_p_)                     \
do{                                                     \
    cxProperty p = cxTypeSetProperty(this,#_p_);        \
    p->setter = (cxPropertySetter)__##_t_##_p_##Setter; \
}while(0)

#define CX_GETTER(_t_,_p_)                     \
do{                                                     \
    cxProperty p = cxTypeSetProperty(this,#_p_);        \
    p->getter = (cxPropertyGetter)__##_t_##_p_##Getter; \
}while(0)

CX_DEF(cxProperty, cxObject)
    cxPropertySetter setter;
    cxPropertyGetter getter;
CX_END(cxProperty, cxObject)

void cxRunPropertySetter(cxAny object,cxConstChars key,cxAny value);

cxBool cxRunPropertyGetter(cxAny object,cxConstChars key,cxAny *value);
CX_C_END

#endif
