//
//  cxProperty.h
//  cxCore
//
//  Created by xuhua on 5/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxProperty_h
#define cxCore_cxProperty_h

#include "cxBase.h"

CX_C_BEGIN

typedef void (*cxPropertySetter)(cxAny,cxAny);

typedef cxAny (*cxPropertyGetter)(cxAny);

#define CX_SETTER(_t_,_p_)          __##_t_##_p_##Setter

#define CX_SETTER_DEF(_t_,_p_)     static void CX_SETTER(_t_,_p_)(_t_ this,cxAny value)

#define CX_GETTER(_t_,_p_)          __##_t_##_p_##Getter

#define CX_GETTER_DEF(_t_,_p_)     static cxAny CX_GETTER(_t_,_p_)(_t_ this)

#define CX_PROPERTY_SETTER(_t_,_p_)                     \
do{                                                     \
    cxProperty p = cxTypeProperty(this,#_p_);           \
    p->setter = (cxPropertySetter)CX_SETTER(_t_,_p_);   \
}while(0)

#define CX_PROPERTY_GETTER(_t_,_p_)                     \
do{                                                     \
    cxProperty p = cxTypeProperty(this,#_p_);           \
    p->getter = (cxPropertyGetter)CX_GETTER(_t_,_p_);   \
}while(0)

CX_OBJECT_DEF(cxProperty, cxObject)
    cxPropertySetter setter;
    cxPropertyGetter getter;
CX_OBJECT_END(cxProperty)

void cxObjectSetter(cxAny object,cxConstChars key,cxAny value);

cxBool cxObjectGetter(cxAny object,cxConstChars key,cxAny *value);
CX_C_END

#endif
