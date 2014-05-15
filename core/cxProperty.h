//
//  cxProperty.h
//  cxEngine
//
//  Created by xuhua on 5/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngineIOS_cxProperty_h
#define cxEngineIOS_cxProperty_h

#include "cxBase.h"

CX_C_BEGIN

typedef void (*cxPropertySetter)(cxAny,cxAny);

typedef cxAny (*cxPropertyGetter)(cxAny);

#define CX_SETTER(_t_,_p_)          __##_t_##_p_##Setter

#define CX_SETTER_DEF(_t_,_p_)     static void CX_SETTER(_t_,_p_)(_t_ this,cxAny value)

#define CX_GETTER(_t_,_p_)          __##_t_##_p_##Getter

#define CX_GETTER_DEF(_t_,_p_)     static cxAny CX_GETTER(_t_,_p_)(_t_ this)

#define CX_PROPERTY_SETTER(_t_,_p_)                                    \
{                                                                      \
    cxProperty p = cxTypeProperty(type,#_p_);                          \
    CX_METHOD_SET(p->setter, (cxPropertySetter)CX_SETTER(_t_,_p_));    \
}

#define CX_PROPERTY_GETTER(_t_,_p_)                                    \
{                                                                      \
    cxProperty p = cxTypeProperty(type,#_p_);                          \
    CX_METHOD_SET(p->getter, (cxPropertyGetter)CX_GETTER(_t_,_p_));    \
}

CX_OBJECT_DEF(cxProperty, cxObject)
    cxPropertySetter setter;
    cxPropertyGetter getter;
CX_OBJECT_END(cxProperty)

CX_C_END

#endif
