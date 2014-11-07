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
#define CX_MCALL(_o_,_n_,_mt_,...)  (_mt_(cxMethodGet(_o_,#_n_)))(_o_,##__VA_ARGS__)

//调用基类的方法
#define CX_SCALL(_o_,_n_,_mt_,...)  (_mt_(cxMethodSuper(_o_,#_n_)))(_o_,##__VA_ARGS__)

//定义一个方法,_t_对象类型，_rt_返回值类型,_n_方法名称,其他为方法参数
#define CX_METHOD(_t_,_n_,...)      _t_##_n_(_t_ this,##__VA_ARGS__)

//设置注册一个方法
#define CX_MSET(_t_,_n_)            cxTypeSetMethod(this,#_n_)->method = (cxAny)_t_##_n_

//确定是否存在方法
#define CX_MHAS(_o_,_n_)            cxMethodHas(_o_,#_n_)

//断言方法存在
#define CX_MASSERT(_o_,_n_)         CX_ASSERT(CX_MHAS(_o_,_n_),"object method %s not exists",#_n_)

CX_DEF(cxMethod, cxObject)
    cxAny method;
CX_END(cxMethod, cxObject)

cxBool cxMethodHas(cxAny object,cxConstChars key);

cxAny cxMethodGet(cxAny object,cxConstChars key);

cxAny cxMethodSuper(cxAny object,cxConstChars key);

CX_C_END

#endif
