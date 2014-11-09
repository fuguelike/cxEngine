//
//  cxButton.c
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxButton.h"

CX_SETTER_DEF(cxButton, movement)
{
    this->movement = cxJsonToDouble(value, this->movement);
}
CX_SETTER_DEF(cxButton, enable)
{
    this->isEnable = cxJsonToBool(value, this->isEnable);
}
CX_METHOD_DEF(cxButton,OnTouch,cxBool,const cxTouchItems *points)
{
    CX_RETURN(points->number != 1,false);
    cxTouchItem item = points->items[0];
    if(!this->isEnable){
        return false;
    }
    cxHitInfo hit = cxViewHitTest(this, item->position);
    if(item->type != cxTouchTypeDown && this->isSelected && !hit.hited){
        CX_EVENT_FIRE(this, onLeave);
        this->isSelected = false;
        return false;
    }
    if(!hit.hited){
        return false;
    }
    if(item->type == cxTouchTypeDown){
        this->isSelected = true;
        CX_EVENT_FIRE(this, onEnter);
        CX_EVENT_FIRE(this, onPress);
        return true;
    }
    if(!this->isSelected){
        return false;
    }
    if(item->movement > this->movement){
        CX_EVENT_FIRE(this, onLeave);
        this->isSelected = false;
        return false;
    }
    if(item->type == cxTouchTypeUp){
        this->isSelected = false;
        CX_EVENT_FIRE(this, onRelease);
        CX_EVENT_FIRE(this, onLeave);
    }
    return true;
}

CX_TYPE(cxButton, cxSprite)
{
    CX_SETTER(cxButton, movement);
    CX_SETTER(cxButton, enable);
    
    CX_METHOD(cxButton, OnTouch);
}
CX_INIT(cxButton, cxSprite)
{
    this->movement = 30;
    this->isEnable = true;
}
CX_FREE(cxButton, cxSprite)
{
    CX_EVENT_RELEASE(this->onEnter);
    CX_EVENT_RELEASE(this->onLeave);
    CX_EVENT_RELEASE(this->onPress);
    CX_EVENT_RELEASE(this->onRelease);
}
CX_TERM(cxButton, cxSprite)

cxButton cxButtonCreate(cxConstChars url)
{
    cxButton this = CX_CREATE(cxButton);
    cxSpriteSetTextureURL(this, url);
    return this;
}

void cxButtonPass(cxAny pview,cxBool pass)
{
    CX_ASSERT_THIS(pview, cxButton);
    this->isPass = pass;
}

void cxButtonEnable(cxAny pview,cxBool enable)
{
    CX_ASSERT_THIS(pview, cxButton);
    this->isEnable = enable;
}


