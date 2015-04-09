//
//  cxButton.c
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxButton.h"


CX_SETTER_DEF(cxButton, enable)
{
    this->IsEnable = cxJsonToBool(value, this->IsEnable);
}
CX_SETTER_DEF(cxButton, ispass)
{
    this->IsPass = cxJsonToBool(value, this->IsPass);
}
CX_METHOD_DEF(cxButton,OnTouch,cxBool,const cxTouchItems *points)
{
    CX_RETURN(points->number != 1,false);
    cxTouchItem item = points->items[0];
    if(!this->IsEnable){
        return false;
    }
    if(item->type == cxTouchTypeMove){
        return this->isSelected && !this->IsPass;
    }
    cxHitInfo hit = cxViewHitTest(this, item->position);
    if(item->type != cxTouchTypeDown && this->isSelected && !hit.hited){
        CX_CALL(this, OnTouchLeave, CX_M(void));
        this->isSelected = false;
        return false;
    }
    if(!hit.hited){
        return false;
    }
    if(item->type == cxTouchTypeDown){
        this->isSelected = true;
        CX_CALL(this, OnTouchEnter, CX_M(void));
        return !this->IsPass;
    }
    if(!this->isSelected){
        return !this->IsPass;
    }
    if(item->type == cxTouchTypeUp){
        this->isSelected = false;
        CX_CALL(this, OnTouchLeave, CX_M(void));
    }
    if(item->type == cxTouchTypeUp){
        CX_CALL(this, OnTouchTap, CX_M(void));
    }
    return !this->IsPass;
}
CX_METHOD_DEF(cxButton, OnTouchTap, void)
{
    CX_EVENT_FIRE(this, onTap);
}
CX_METHOD_DEF(cxButton, OnTouchEnter, void)
{
    
}
CX_METHOD_DEF(cxButton, OnTouchLeave, void)
{
    
}
CX_TYPE(cxButton, cxSprite)
{
    CX_SETTER(cxButton, enable);
    CX_SETTER(cxButton, ispass);
    
    CX_METHOD(cxButton, OnTouch);
    CX_METHOD(cxButton, OnTouchTap);
    CX_METHOD(cxButton, OnTouchEnter);
    CX_METHOD(cxButton, OnTouchLeave);
}
CX_INIT(cxButton, cxSprite)
{
    this->IsEnable = true;
    this->IsPass = false;
}
CX_FREE(cxButton, cxSprite)
{
    CX_EVENT_RELEASE(this->onTap);
}
CX_TERM(cxButton, cxSprite)



