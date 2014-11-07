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

//方法描述(_rt_,返回值类型，其他为参数)
#define CX_MT(_rt_,...)             (_rt_ (*)(cxAny,##__VA_ARGS__))

//调用的方法必须与CX_DEFINE描述一致
#define CX_CALL(_o_,_n_,_mt_,...)   (_mt_(cxMethodGet(_o_,#_n_)))(_o_,##__VA_ARGS__)

//调用基类的方法
#define CX_BASE(_o_,_n_,_mt_,...)   (_mt_(cxMethodSuper(_o_,#_n_)))(_o_,##__VA_ARGS__)

//定义一个方法,_t_对象类型，_rt_返回值类型,_n_方法名称,其他为方法参数
#define CX_DEFINE(_t_,_n_,_rt_,...) static _rt_ _t_##_n_(_t_ this,##__VA_ARGS__)

//注册一个方法
#define CX_METHOD(_t_,_n_)          cxTypeSetMethod(this,#_n_)->method = (cxAny)_t_##_n_

CX_DEF(cxMethod, cxObject)
    cxAny method;
CX_END(cxMethod, cxObject)

cxAny cxMethodGet(cxAny object,cxConstChars key);

cxAny cxMethodSuper(cxAny object,cxConstChars key);

CX_C_END

#endif
