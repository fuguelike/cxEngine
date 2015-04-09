//
//  cxProgress.h
//  cxEngineStatic
//
//  Created by xuhua on 1/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngineStatic_cxProgress_h
#define cxEngineStatic_cxProgress_h

#include <views/cxAtlas.h>

CX_C_BEGIN

typedef enum {
    cxProgressTypeNone          = 0,        //
    cxProgressTypeHorizontal    = 1 << 0,   //
    cxProgressTypeVertical      = 1 << 1,   //
}cxProgressType;

CX_DEF(cxProgress, cxView)
    CX_FIELD_DEF(cxInt ActiveIdx);      //cxAtlas view
    CX_FIELD_DEF(cxInt StaticIdx);      //cxAtlas view
    CX_FIELD_DEF(cxFloat Value);
    CX_FIELD_DEF(cxRange2f Range);
    CX_FIELD_DEF(cxProgressType Type);
CX_END(cxProgress, cxView)

CX_FIELD_GET(cxProgress, cxProgressType, Type);
CX_INLINE void cxProgressSetType(cxAny pthis,const cxProgressType value)
{
    CX_ASSERT_THIS(pthis, cxProgress);
    CX_RETURN(cxFloatEqu(this->Type, value));
    this->Type = value;
    cxViewSetDirty(this, cxViewDirtyForce);
}
CX_FIELD_SET(cxProgress, cxInt, ActiveIdx);
CX_FIELD_SET(cxProgress, cxInt, StaticIdx);

CX_FIELD_GET(cxProgress, cxFloat, Value);
CX_INLINE void cxProgressSetValue(cxAny pthis,const cxFloat value)
{
    CX_ASSERT_THIS(pthis, cxProgress);
    cxFloat fix = kmClamp(value, this->Range.min, this->Range.max);
    CX_RETURN(cxFloatEqu(this->Value, fix));
    this->Value = fix;
    cxViewSetDirty(this, cxViewDirtyForce);
}
CX_FIELD_GET(cxProgress, cxRange2f, Range);
CX_INLINE void cxProgressSetRange(cxAny pthis,const cxRange2f value)
{
    CX_ASSERT_THIS(pthis, cxProgress);
    CX_RETURN(cxRange2fEqu(this->Range, value));
    this->Range = value;
    this->Value = kmClamp(this->Value, this->Range.min, this->Range.max);
    cxViewSetDirty(this, cxViewDirtyForce);
}

cxAny cxProgressGetStatic(cxAny pview);

cxAny cxProgressGetActive(cxAny pview);

void cxProgressUpdate(cxAny pview);

CX_C_END

#endif
