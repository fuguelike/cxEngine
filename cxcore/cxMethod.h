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

//GET METHOD DESC _rt_(return value type)
#define CX_MT(_rt_,...)             (_rt_ (*)(cxAny,##__VA_ARGS__))
//CALL METHOD
#define CX_CALL(_o_,_n_,_t_,...)    (_t_(cxMethodGet(_o_,#_n_)))(_o_,##__VA_ARGS__)
//CALL BASE METHOD
#define CX_BASE(_o_,_n_,_t_,...)    (_t_(cxMethodSuper(_o_,#_n_)))(_o_,##__VA_ARGS__)
//DEINFE METHOD
#define CX_DEFINE(_t_,_r_,_n_,...)  static _r_ _t_##_n_(_t_ this,##__VA_ARGS__)
//REG METHOD
#define CX_METHOD(_t_,_n_)          cxTypeSetMethod(this,#_n_)->method = (cxAny)_t_##_n_

CX_DEF(cxMethod, cxObject)
    cxAny method;
CX_END(cxMethod, cxObject)

cxAny cxMethodGet(cxAny object,cxConstChars key);

cxAny cxMethodSuper(cxAny object,cxConstChars key);

CX_C_END

#endif
