//
//  cxButton.c
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxButton.h"

cxBool cxButtonTouch(cxAny pview,cxTouchItems *fires,cxTouchItems *points)
{
    CX_RETURN(CX_TOUCH_SIZE(points) != 1,false);
    cxTouchItem item = CX_TOUCH_ITEM(fires, 0);
    cxButton this = pview;
    if(!this->isEnable){
        return false;
    }
    cxBool hit = cxViewHitTest(pview, item->current, NULL);
    //leave
    if(item->type == cxTouchTypeMove && this->isSelected && !hit){
        CX_EVENT_FIRE(this, onLeave);
        this->isSelected = false;
        return false;
    }
    if(!hit){
        return false;
    }
    if(item->type == cxTouchTypeDown){
        this->isSelected = true;
        CX_EVENT_FIRE(this, onEnter);
        CX_EVENT_FIRE(this, onPress);
        return false;
    }
    if(!this->isSelected){
        CX_EVENT_FIRE(this, onLeave);
        this->isSelected = false;
        return false;
    }
    if(item->type == cxTouchTypeUp){
        this->isSelected = false;
        CX_EVENT_FIRE(this, onRelease);
        CX_EVENT_FIRE(this, onLeave);
        return true;
    }
    return this->isSelected;
}

CX_SETTER_DEF(cxButton, movement)
{
    this->movement = cxJsonToDouble(value, this->movement);
}
CX_SETTER_DEF(cxButton, enable)
{
    this->isEnable = cxJsonToBool(value, this->isEnable);
}

CX_OBJECT_TYPE(cxButton, cxSprite)
{
    CX_PROPERTY_SETTER(cxButton, movement);
    CX_PROPERTY_SETTER(cxButton, enable);
}
CX_OBJECT_INIT(cxButton, cxSprite)
{
    this->movement = 25;
    this->isEnable = true;
    CX_METHOD_SET(this->cxSprite.cxView.Touch, cxButtonTouch);
}
CX_OBJECT_FREE(cxButton, cxSprite)
{
    CX_EVENT_RELEASE(this->onEnter);
    CX_EVENT_RELEASE(this->onLeave);
    CX_EVENT_RELEASE(this->onPress);
    CX_EVENT_RELEASE(this->onRelease);
}
CX_OBJECT_TERM(cxButton, cxSprite)

cxButton cxButtonCreate(cxConstChars url)
{
    cxButton this = CX_CREATE(cxButton);
    cxSpriteSetTextureURL(this, url, true);
    return this;
}

void cxButtonEnable(cxAny pview,cxBool enable)
{
    cxButton this = pview;
    this->isEnable = enable;
}


