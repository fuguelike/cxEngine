//
//  cxBezier.c
//  cxEngine
//
//  Created by xuhua on 8/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <views/cxSprite.h>
#include "cxBezier.h"

static void cxBezierUpdateAngle(cxBezier this,cxVec2f npos,cxVec2f opos)
{
    cxFloat angle = cxVec2fRadiansBetween(npos, opos);
    if(!cxFloatEqu(angle, this->Angle)){
        this->Angle = angle;
        CX_CALL(this, OnAngle, CX_M(void));
    }
}
CX_METHOD_DEF(cxBezier,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxVec2f opos = cxViewGetPosition(view);
    this->from = cxVec2fAdd(opos, this->Offset);
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxBezier,Step,void,cxFloat dt,cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxVec2f curr = cxViewGetPosition(view);
    cxVec2f npos;
    if(this->Type == cxBezierType3){
        npos.x = cxBezier3(this->from.x, this->Pos1.x, this->Pos2.x, this->Epos.x, time);
        npos.y = cxBezier3(this->from.y, this->Pos1.y, this->Pos2.y, this->Epos.y, time);
    }else if(this->Type == cxBezierType2){
        npos.x = cxBezier2(this->from.x, this->Pos1.x, this->Epos.x, time);
        npos.y = cxBezier2(this->from.y, this->Pos1.y, this->Epos.y, time);
    }
    cxBezierUpdateAngle(this, npos, curr);
    cxViewSetPosition(view, npos);
}
CX_SETTER_DEF(cxBezier, epos)
{
    this->Epos = cxJsonToVec2f(value, this->Epos);
}
CX_SETTER_DEF(cxBezier, pos1)
{
    this->Pos1 = cxJsonToVec2f(value, this->Pos1);
}
CX_SETTER_DEF(cxBezier, pos2)
{
    this->Pos2 = cxJsonToVec2f(value, this->Pos2);
}
CX_METHOD_DEF(cxBezier, OnAngle, void)
{
    
}
CX_TYPE(cxBezier, cxAction)
{
    CX_SETTER(cxBezier, epos);
    CX_SETTER(cxBezier, pos1);
    CX_SETTER(cxBezier, pos2);
    
    CX_METHOD(cxBezier, OnAngle);
    CX_METHOD(cxBezier, Init);
    CX_METHOD(cxBezier, Step);
}
CX_INIT(cxBezier, cxAction)
{
    this->Angle = INT32_MAX;
    cxBezierSetType(this, cxBezierType3);
}
CX_FREE(cxBezier, cxAction)
{
    
}
CX_TERM(cxBezier, cxAction)

cxBezier cxBezier2Create(cxFloat time,cxVec2f epos,cxVec2f cpos1)
{
    cxBezier this = CX_CREATE(cxBezier);
    cxActionSetTime(this, time);
    cxBezierSetType(this, cxBezierType2);
    this->Epos = epos;
    this->Pos1 = cpos1;
    return this;
}

cxBezier cxBezier3Create(cxFloat time,cxVec2f epos,cxVec2f cpos1,cxVec2f cpos2)
{
    cxBezier this = CX_CREATE(cxBezier);
    cxActionSetTime(this, time);
    cxBezierSetType(this, cxBezierType3);
    this->Epos = epos;
    this->Pos1 = cpos1;
    this->Pos2 = cpos2;
    return this;
}








