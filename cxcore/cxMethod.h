//
//  cxMethod.h
//  cxCore
//
//  Created by xuhua on 11/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxMethod_h
#define cxCore_cxMethod_h

#include "cxCore.h"

CX_C_BEGIN

//GET METHOD ARGS
#define CX_T(_rt_,...)                          (_rt_ (*)(cxAny,##__VA_ARGS__))
//CALL METHOD
#define CX_C(_o_,_n_,_ft_,...)                  (_ft_(cxMethodGet(_o_,#_n_)))(_o_,##__VA_ARGS__)
//CALL SUPER METHOD
#define CX_S(_o_,_n_,_ft_,...)                  (_ft_(cxMethodSuper(_o_,#_n_)))(_o_,##__VA_ARGS__)
//DEINF METHOD
#define CX_DEFINE(_t_,_r_,_n_,...)              static _r_ _t_##_n_(_t_ this,##__VA_ARGS__)
//REGMETHOD
#define CX_METHOD(_t_,_n_)                      \
do{                                             \
    cxMethod m = cxTypeSetMethod(this,#_n_);    \
    m->method = (cxAny)_t_##_n_;                \
}while(0)

CX_DEF(cxMethod, cxObject)
    cxAny method;
CX_END(cxMethod, cxObject)

cxAny cxMethodGet(cxAny object,cxConstChars key);

cxAny cxMethodSuper(cxAny object,cxConstChars key);

CX_C_END

#endif
