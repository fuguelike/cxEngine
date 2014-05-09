//
//  cxButton.c
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxButton.h"

cxBool cxButtonTouch(cxAny pview,cxTouch *touch)
{
    cxButton this = pview;
    if(!this->isEnable){
        return false;
    }
    cxBool hit = cxViewHitTest(pview, touch->current, NULL);
    if(!hit && this->isDown){
        CX_EVENT_FIRE(this, onLeave);
        this->isDown = false;
    }
    if(!hit){
        return false;
    }
    if(touch->type == cxTouchTypeDown){
        this->isDown = true;
        CX_EVENT_FIRE(this, onEnter);
        CX_EVENT_FIRE(this, onPress);
        return true;
    }
    if(!this->isDown){
        return false;
    }
    if(touch->type == cxTouchTypeMove && cxVec2fMagnitude(touch->movement) > this->movement){
        CX_EVENT_FIRE(this, onLeave);
        this->isDown = false;
        return false;
    }
    if(touch->type == cxTouchTypeUp){
        this->isDown = false;
        CX_EVENT_FIRE(this, onRelease);
        CX_EVENT_FIRE(this, onLeave);
        return true;
    }
    return false;
}

void __cxButtonInitObject(cxAny object,cxAny json,cxAny hash)
{
    CX_OBJECT_INIT_SUPER(cxSprite);
}

CX_OBJECT_INIT(cxButton, cxSprite)
{
    this->movement = 25;
    this->isEnable = true;
    CX_METHOD_SET(this->super.super.Touch, cxButtonTouch);
    CX_OBJECT_INIT_OVERRIDE(cxButton, this);
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


