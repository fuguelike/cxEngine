//
//  cxProgress.c
//  cxEngineStatic
//
//  Created by xuhua on 1/30/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxProgress.h"

void cxProgressUpdate(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxProgress);
    this->Value = kmClamp(this->Value, this->Range.min, this->Range.max);
    cxFloat scale = this->Value / this->Range.max;
    cxAny sv = cxProgressGetStatic(this);
    cxAny av = cxProgressGetActive(this);
    if(sv == NULL || av == NULL){
        CX_ERROR("active view and static view miss");
        return;
    }
    if(cxFloatEqu(scale, 0)){
        cxViewSetIsVisible(av, false);
        return;
    }
    cxViewSetIsVisible(av, true);
    
    cxBox4f sbox;
    if(cxInstanceOf(sv, cxAtlasTypeName)){
        sbox = cxAtlasGetScale9Box(sv);
    }else{
        sbox = cxViewGetAutoBox(sv);
    }
    cxSize2f ssiz = cxViewGetSize(sv);
    
    cxBox4f abox;
    if(cxInstanceOf(av, cxAtlasTypeName)){
        abox = cxAtlasGetScale9Box(av);
    }else{
        abox = cxViewGetAutoBox(av);
    }
    cxSize2f asiz = cxViewGetSize(av);
    
    cxVec2f pos = cxViewGetPosition(av);
    cxFloat wv = asiz.w;
    cxFloat hv = asiz.h;
    cxFloat sw = 0;
    cxFloat sh = 0;
    
    if(this->Type & cxProgressTypeHorizontal){
        sw = ssiz.w - sbox.l - sbox.r;
        wv = sw * scale;
        pos.x = -ssiz.w/2.0f + wv/2.0f + sbox.l;
        wv += abox.l + abox.r;
    }
    if(this->Type & cxProgressTypeVertical){
        sh = ssiz.h - sbox.t - sbox.b;
        hv = sh * scale;
        pos.y = -ssiz.h/2.0f + hv/2.0f + sbox.t;
        hv += abox.t + abox.b;
    }
    cxViewSetPosition(av, pos);
    cxViewSetSize(av, cxSize2fv(wv, hv));
}
CX_METHOD_DEF(cxProgress, OnDirty, void)
{
    CX_SUPER(cxView, this, OnDirty, CX_M(void));
    cxViewDirty dirty = cxViewGetDirty(this);
    if(dirty & cxViewDirtyForce){
        cxProgressUpdate(this);
    }
}
CX_SETTER_DEF(cxProgress, active)
{
    cxProgressSetActiveIdx(this, cxJsonToInt(value, this->ActiveIdx));
}
CX_SETTER_DEF(cxProgress, static)
{
    cxProgressSetStaticIdx(this, cxJsonToInt(value, this->StaticIdx));
}
CX_SETTER_DEF(cxProgress, value)
{
    cxProgressSetValue(this, cxJsonToDouble(value, this->Value));
}
CX_SETTER_DEF(cxProgress, range)
{
    cxProgressSetRange(this, cxJsonToRange2f(value, this->Range));
}
CX_SETTER_DEF(cxProgress, type)
{
    cxConstChars stype = cxJsonToConstChars(value);
    cxProgressType type = this->Type;
    if(cxConstCharsHas(stype, "vertical")){
        type |= cxProgressTypeVertical;
    }
    if(cxConstCharsHas(stype, "horizontal")){
        type |= cxProgressTypeHorizontal;
    }
    cxProgressSetType(this, type);
}
CX_TYPE(cxProgress, cxView)
{
    CX_SETTER(cxProgress, type);
    CX_SETTER(cxProgress, active);
    CX_SETTER(cxProgress, static);
    CX_SETTER(cxProgress, value);
    CX_SETTER(cxProgress, range);
    
    CX_METHOD(cxProgress, OnDirty);
}
CX_INIT(cxProgress, cxView)
{
    this->StaticIdx = 0;
    this->ActiveIdx = 1;
    this->Range = cxRange2fv(0, 1);
    this->Type = cxProgressTypeHorizontal;
}
CX_FREE(cxProgress, cxView)
{
    
}
CX_TERM(cxProgress, cxView)

cxAny cxProgressGetStatic(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxProgress);
    cxList list = cxViewGetSubViews(this);
    return cxListAt(list, this->StaticIdx);
}

cxAny cxProgressGetActive(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxProgress);
    cxList list = cxViewGetSubViews(this);
    return cxListAt(list, this->ActiveIdx);
}