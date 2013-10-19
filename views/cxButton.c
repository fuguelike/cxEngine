//
//  cxButton.c
//  cxEngine
//
//  Created by xuhua on 10/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxViewXML.h"
#include "cxButton.h"
#include "cxXMLScript.h"

cxBool cxButtonTouch(cxAny pview,const cxTouch *touch)
{
    cxButton this = pview;
    if(!this->isEnable){
        return false;
    }
    cxBool hit = cxViewHitTest(pview, touch, &this->touchPos);
    if(!hit && this->isDown){
        CX_EVENT_FIRE(this, onLeave);
        this->isDown = false;
    }
    if(!hit){
        return false;
    }
    if(touch->type == cxTouchTypeDown){
        this->isDown = true;
        CX_EVENT_FIRE(this, onPress);
        return true;
    }
    if(touch->type == cxTouchTypeMove && this->isDown){
        CX_EVENT_FIRE(this, onMove);
        return true;
    }
    if(touch->type == cxTouchTypeUp && this->isDown){
        this->isDown = false;
        CX_EVENT_FIRE(this, onRelease);
        CX_EVENT_FIRE(this, onLeave);
        return true;
    }
    if(touch->type == cxTouchTypeCancel){
        this->isDown = false;
    }
    return false;
}

void cxButtonXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewXML xml = xmlView;
    cxButton this = mView;
    cxSpriteXMLReadAttr(xmlView, mView, reader);
    cxXMLAppendEvent(xml->events, this, cxButton, onPress);
    cxXMLAppendEvent(xml->events, this, cxButton, onMove);
    cxXMLAppendEvent(xml->events, this, cxButton, onRelease);
    cxXMLAppendEvent(xml->events, this, cxButton, onLeave);
    cxButtonEnable(this, cxXMLReadBoolAttr(reader, "cxButton.enable", this->isEnable));
}

CX_OBJECT_INIT(cxButton, cxSprite)
{
    cxObjectSetXMLReadFunc(this, cxButtonXMLReadAttr);
    this->isEnable = true;
    CX_METHOD_SET(this->super.super.Touch, cxButtonTouch);
}
CX_OBJECT_FREE(cxButton, cxSprite)
{
    CX_EVENT_RELEASE(this->onLeave);
    CX_EVENT_RELEASE(this->onPress);
    CX_EVENT_RELEASE(this->onMove);
    CX_EVENT_RELEASE(this->onRelease);
}
CX_OBJECT_TERM(cxButton, cxSprite)

void cxButtonEnable(cxAny pview,cxBool enable)
{
    cxButton this = pview;
    this->isEnable = enable;
}


