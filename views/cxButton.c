//
//  cxButton.c
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxViewRoot.h"
#include "cxButton.h"
#include "cxXMLScript.h"

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

void cxButtonReadAttr(cxAny rootView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewRoot root = rootView;
    cxButton this = mView;
    cxSpriteReadAttr(rootView, mView, reader);
    cxXMLAppendEvent(root->events, this, cxButton, onPress);
    cxXMLAppendEvent(root->events, this, cxButton, onRelease);
    cxXMLAppendEvent(root->events, this, cxButton, onLeave);
    cxXMLAppendEvent(root->events, this, cxButton, onEnter);
    cxButtonEnable(this, cxXMLReadBoolAttr(reader, root->functions, "cxButton.enable", this->isEnable));
    this->movement = cxXMLReadFloatAttr(reader, root->functions,  "cxButton.movement", this->movement);
}

CX_OBJECT_INIT(cxButton, cxSprite)
{
    cxObjectSetReadAttrFunc(this, cxButtonReadAttr);
    this->movement = 15;
    this->isEnable = true;
    CX_METHOD_SET(this->super.super.Touch, cxButtonTouch);
}
CX_OBJECT_FREE(cxButton, cxSprite)
{
    CX_EVENT_RELEASE(this->onEnter);
    CX_EVENT_RELEASE(this->onLeave);
    CX_EVENT_RELEASE(this->onPress);
    CX_EVENT_RELEASE(this->onRelease);
}
CX_OBJECT_TERM(cxButton, cxSprite)

void cxButtonEnable(cxAny pview,cxBool enable)
{
    cxButton this = pview;
    this->isEnable = enable;
}


