//
//  cxCircle.c
//  cxEngineStatic
//
//  Created by xuhua on 2/2/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxCircle.h"

CX_METHOD_DEF(cxCircle, OnDirty, void)
{
    CX_SUPER(cxSprite, this, OnDirty, CX_M(void));
    cxBoxTex2f tbox = cxSpriteBoxTex(this);
    cxFloat tw = tbox.rb.u - tbox.lb.u;
    cxFloat th = tbox.rb.v - tbox.rt.v;
    cxFloat tx = tbox.lt.u;
    cxFloat ty = tbox.lt.v;
    cxFloat cu = tx + tw /2.0f;
    cxFloat cv = ty + th / 2.0f;
    
    cxColor4f color = cxViewGetColor(this);
    cxSize2f size = cxViewGetSize(this);
    cxFloat hw = size.w/2.0f;
    cxFloat hh = size.h/2.0f;
    
    cxFloat r = sqrtf(hw * hw + hh * hh);
    
    this->number = 1;
    this->vs[0] = cxVec3fv(0, 0, 0);
    
    cxFloat angle = this->Start;
    cxFloat ax = this->Percent * 360.0f;
    
    cxBool quit = false;
    cxFloat dv;
    if(angle < 45){
        dv = this->IsReverse?angle:(45 - angle);
    }else{
        dv = 45;
    }
    for(cxInt i = 1; i < 11;i++){
        cxFloat ar = kmDegreesToRadians(angle);
        cxFloat x = cosf(ar) * r;
        cxFloat y = sinf(ar) * r;
        if(y > hh){
            y = hh;
        }
        if(y < -hh){
            y = -hh;
        }
        if(x > hw){
            x = hw;
        }
        if(x < -hw){
            x = -hw;
        }
        this->vs[i] = cxVec3fv(x, y, 0);
        this->number++;
        if(quit)break;
        if(ax - dv < 0.0f){
            dv = ax;
            quit = true;
        }
        if(this->IsReverse){
            angle -= dv;
        }else{
            angle += dv;
        }
        ax -= dv;
        dv = 45;
    }
    for(cxInt j=0; j < this->number;j++){
        cxFloat u = cu + (this->vs[j].x / hw) * (tw / 2.0f);
        cxFloat v = cv - (this->vs[j].y / hh) * (th / 2.0f);
        this->ts[j] = cxTex2fv(u, v);
        this->cs[j] = color;
    }
}
CX_METHOD_DEF(cxCircle, OnDraw, void)
{
    if(!cxSpriteBindTexture(this)){
        return;
    }
    OpenGLDrawTriangle(GL_TRIANGLE_FAN, this->number, this->vs, this->cs, this->ts);
}
CX_SETTER_DEF(cxCircle, percent)
{
    cxCircleSetPercent(this, cxJsonToDouble(value, 0.3f));
}
CX_SETTER_DEF(cxCircle, start)
{
    cxCircleSetStart(this, cxJsonToDouble(value, 90));
}
CX_SETTER_DEF(cxCircle, reverse)
{
    cxCircleSetIsReverse(this, cxJsonToBool(value, true));
}
CX_METHOD_DEF(cxCircle, VertexNumber, cxInt)
{
    return 11;
}
CX_TYPE(cxCircle, cxSprite)
{
    CX_SETTER(cxCircle, percent);
    CX_SETTER(cxCircle, start);
    CX_SETTER(cxCircle, reverse);
    
    CX_METHOD(cxCircle, VertexNumber);
    CX_METHOD(cxCircle, OnDraw);
    CX_METHOD(cxCircle, OnDirty);
}
CX_INIT(cxCircle, cxSprite)
{
    this->Percent = 0.3f;
    this->IsReverse = true;
    this->Start = 90;
    cxInt num = CX_CALL(this, VertexNumber, CX_M(cxInt));
    this->vs = allocator->malloc(sizeof(cxVec3f) * num);
    this->cs = allocator->malloc(sizeof(cxColor4f) * num);
    this->ts = allocator->malloc(sizeof(cxTex2f) * num);
}
CX_FREE(cxCircle, cxSprite)
{
    allocator->free(this->ts);
    allocator->free(this->vs);
    allocator->free(this->cs);
}
CX_TERM(cxCircle, cxSprite)