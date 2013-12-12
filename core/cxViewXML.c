//
//  cxViewXML.c
//  cxEngine
//  use attr x y w h ax ay al ar at ab texture file
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <ctype.h>
#include <core/cxEngine.h>
#include <core/cxStack.h>
#include <core/cxAutoPool.h>
#include <views/cxButton.h>
#include <views/cxAtlas.h>
#include <views/cxParticle.h>
#include <views/cxLabelTTF.h>
#include <views/cxLabelBMP.h>
#include <views/cxClipping.h>
#include <views/cxTable.h>
#include <views/cxScroll.h>
#include <views/cxChipmunk.h>
#include "cxViewXML.h"
#include "cxHashXML.h"
#include "cxActionXML.h"

void cxViewRootXMLReadAttr(cxAny pxml,cxAny view, xmlTextReaderPtr reader)
{
    cxViewXMLReadAttr(pxml, view, reader);
    cxViewXML this = view;
    cxViewXML xml = pxml;
    cxXMLAppendEvent(xml->events, this, cxViewXML, onBegin);
    cxXMLAppendEvent(xml->events, this, cxViewXML, onEnd);
}

void cxViewXMLRegisteEvent(cxAny pview,cxConstChars name,cxEventFunc func)
{
    cxViewXML this = pview;
    if(func == NULL){
        cxHashDel(this->events, cxHashStrKey(name));
        return;
    }
    cxEventItem event = cxHashGet(this->events, cxHashStrKey(name));
    CX_ASSERT(event == NULL, "name %s event registered",name);
    event = CX_ALLOC(cxEventItem);
    event->func = func;
    cxHashSet(this->events, cxHashStrKey(name), event);
    CX_RELEASE(event);
}

void cxViewXMLRegisteFunc(cxAny pview,cxConstChars name,cxAny func)
{
    cxViewXML this = pview;
    if(func == NULL){
        cxHashDel(this->functions, cxHashStrKey(name));
        return;
    }
    cxFuncItem item = cxHashGet(this->functions, cxHashStrKey(name));
    CX_ASSERT(item == NULL, "name %s function registered",name);
    item = CX_ALLOC(cxFuncItem);
    item->func = func;
    cxHashSet(this->events, cxHashStrKey(name), item);
    CX_RELEASE(item);
}

void cxViewXMLSetItem(cxAny pview,cxConstChars key,cxAny item)
{
    cxViewXML this = pview;
    cxHashSet(this->items, cxHashStrKey(key), item);
}

CX_OBJECT_INIT(cxViewXML, cxView)
{
    cxObjectSetXMLReadFunc(this, cxViewRootXMLReadAttr);
    CX_METHOD_SET(this->Make, cxViewXMLMakeElement);
    this->items = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->actions = CX_ALLOC(cxHash);
    this->functions = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxViewXML, cxView)
{
    CX_EVENT_RELEASE(this->onBegin);
    CX_EVENT_RELEASE(this->onEnd);
    CX_RELEASE(this->functions);
    CX_RELEASE(this->items);
    CX_RELEASE(this->events);
    CX_RELEASE(this->actions);
}
CX_OBJECT_TERM(cxViewXML, cxView)

cxAny cxViewXMLGet(cxAny pview,cxConstChars key)
{
    cxViewXML xml = cxObjectRoot(pview);
    return (xml != NULL) ? cxHashGet(xml->items, cxHashStrKey(key)) : NULL;
}

void cxViewXMLRemove(cxAny pview,cxConstChars key)
{
    cxViewXML this = pview;
    cxAny view = cxHashGet(this->items, cxHashStrKey(key));
    CX_RETURN(view == NULL);
    cxViewRemoved(view);
    cxHashDel(this->items, cxHashStrKey(key));
}

void cxViewReplaceViewEvent(cxEvent *event)
{
    CX_ASSERT(event->args != NULL, "args error");
    cxConstChars url = cxEventArgToString(event->args);
    CX_ASSERT(url != NULL, "args error");
    cxViewXML view = cxViewXMLCreate(url);
    CX_ASSERT(view != NULL, "create xml view %s falied ",url);
    cxWindowReplaceView(view,event->args);
}

void cxViewPushViewEvent(cxEvent *event)
{
    CX_ASSERT(event->args != NULL, "args error");
    cxConstChars url = cxEventArgToString(event->args);
    CX_ASSERT(url != NULL, "args error");
    cxViewXML view = cxViewXMLCreate(url);
    CX_ASSERT(view != NULL, "create xml view %s falied ",url);
    cxWindowPushView(view,event->args);
}

void cxViewPopViewEvent(cxEvent *event)
{
    cxWindowPopView(event->args);
}

cxViewXML cxViewXMLCreate(cxConstChars xml)
{
    cxViewXML this = CX_CREATE(cxViewXML);
    if(!cxViewXMLLoad(this, xml)){
        return NULL;
    }
    cxObjectSetRoot(this, this);
    return this;
}

void cxViewXMLSet(cxAny pview,cxAny cview,xmlTextReaderPtr reader)
{
    cxViewXML this = pview;
    cxConstChars id = cxXMLAttr("id");
    CX_RETURN(id == NULL);
    cxHashSet(this->items, cxHashStrKey(id), cview);
}

cxAny cxViewXMLMakeElement(cxConstChars temp,xmlTextReaderPtr reader)
{
    cxView cview = NULL;
    if(ELEMENT_IS_TYPE(cxSprite)){
        cview = CX_CREATE(cxSprite);
    }else if(ELEMENT_IS_TYPE(cxViewXML)){
        cxConstChars src = cxXMLAttr("src");
        CX_ASSERT(src != NULL, "require src attr");
        cview = (cxView)cxViewXMLCreate(src);
    }else if(ELEMENT_IS_TYPE(cxView)){
        cview = CX_CREATE(cxView);
    }else if(ELEMENT_IS_TYPE(cxButton)){
        cview = CX_CREATE(cxButton);
    }else if(ELEMENT_IS_TYPE(cxAtlas)){
        cview = CX_CREATE(cxAtlas);
    }else if(ELEMENT_IS_TYPE(cxParticle)){
        cview = CX_CREATE(cxParticle);
    }else if(ELEMENT_IS_TYPE(cxLabelTTF)){
        cview = CX_CREATE(cxLabelTTF);
    }else if(ELEMENT_IS_TYPE(cxLabelBMP)){
        cview = CX_CREATE(cxLabelBMP);
    }else if(ELEMENT_IS_TYPE(cxClipping)){
        cview = CX_CREATE(cxClipping);
    }else if(ELEMENT_IS_TYPE(cxTable)){
        cview = CX_CREATE(cxTable);
    }else if(ELEMENT_IS_TYPE(cxScroll)){
        cview = CX_CREATE(cxScroll);
    }else if(ELEMENT_IS_TYPE(cxChipmunk)){
        cview = CX_CREATE(cxChipmunk);
    }else{
        CX_ERROR("make elemement %s error",temp);
    }
    return cview;
}

static void cxViewXMLLoadSubviews(cxAny pview,xmlTextReaderPtr reader,cxStack stack)
{
    cxViewXML this = pview;
    while(xmlTextReaderRead(reader)){
        int type = xmlTextReaderNodeType(reader);
        if(type == XML_READER_TYPE_ELEMENT){
            cxView parent = cxStackTop(stack);
            if(parent == NULL){
                CX_ERROR("parse xml ui parent null");
                break;
            }
            cxConstChars temp = cxXMLReadElementName(reader);
            cxView cview = CX_METHOD_GET(NULL, this->Make, temp, reader);
            CX_ASSERT(cview != NULL, "make element null");
            cxObjectSetRoot(cview, this);
            cxViewAppend(parent, cview);
            cxObjectXMLReadAttrRun(cview, this, reader);
            cxViewXMLSet(this, cview, reader);
            if(xmlTextReaderIsEmptyElement(reader)){
                continue;
            }
            cxStackPush(stack, cview);
        }else if(type == XML_READER_TYPE_END_ELEMENT){
            cxStackPop(stack);
        }
    }
}

static void cxViewXMLReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxViewXML this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

cxBool cxViewXMLLoadWithReader(cxAny pview,xmlTextReaderPtr reader)
{
    cxBool ret = false;
    cxViewXML xmlView = pview;
    cxStack stack = CX_ALLOC(cxStack);
    while(xmlTextReaderRead(reader) && !xmlView->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(ELEMENT_IS_TYPE(cxViewXML)){
            ret = true;
            break;
        }
    }
    if(ret){
        cxAutoPoolPush();
        cxObjectXMLReadAttrRun(xmlView, xmlView, reader);
        cxStackPush(stack, xmlView);
        CX_EVENT_FIRE(xmlView, onBegin);
        cxViewXMLLoadSubviews(xmlView,reader, stack);
        CX_EVENT_FIRE(xmlView, onEnd);
        cxStackPop(stack);
        cxAutoPoolPop();
    }
    CX_RELEASE(stack);
    return ret;
}

cxBool cxViewXMLLoad(cxAny pview,cxConstChars xml)
{
    cxBool ret = false;
    cxXMLScript script = cxEngineGetXMLScript(xml);
    if(script == NULL || script->bytes == NULL){
        CX_ERROR("%s script not register",xml);
        return ret;
    }
    xmlTextReaderPtr reader = cxXMLReaderForScript(script,cxViewXMLReaderError,pview);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return ret;
    }
    ret = cxViewXMLLoadWithReader(pview, reader);
    return ret;
}











