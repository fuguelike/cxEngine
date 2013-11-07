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
#include <views/cxLabel.h>
#include <views/cxClipping.h>
#include <views/cxTable.h>
#include <views/cxScroll.h>
#include "cxViewXML.h"
#include "cxHashXML.h"
#include "cxActionXML.h"

cxBool cxViewRootXMLReadAttr(cxAny pxml,cxAny view, xmlTextReaderPtr reader)
{
    cxViewXMLReadAttr(pxml, view, reader);
    cxViewXML this = view;
    cxViewXML xml = pxml;
    cxXMLAppendEvent(xml->events, this, cxViewXML, onLoad);
    return true;
}

void cxViewXMLRegisteEvent(cxAny pview,cxConstChars name,cxEventFunc func)
{
    cxViewXML this = pview;
    if(func == NULL){
        cxHashDel(this->events, cxHashStrKey(name));
    }else{
        cxEventItem event = cxHashGet(this->events, cxHashStrKey(name));
        CX_ASSERT(event == NULL, "name %s event registered",name);
        event = CX_ALLOC(cxEventItem);
        event->func = func;
        cxHashSet(this->events, cxHashStrKey(name), event);
        CX_RELEASE(event);
    }
}

CX_OBJECT_INIT(cxViewXML, cxView)
{
    cxObjectSetXMLReadFunc(this, cxViewRootXMLReadAttr);
    CX_METHOD_SET(this->Make, cxViewXMLMakeElement);
    this->items = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->actions = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxViewXML, cxView)
{
    CX_EVENT_RELEASE(this->onLoad);
    CX_RELEASE(this->items);
    CX_RELEASE(this->events);
    CX_RELEASE(this->actions);
}
CX_OBJECT_TERM(cxViewXML, cxView)

cxAny cxViewXMLGet(cxAny pview,cxConstChars key)
{
    cxAny root = cxObjectRoot(pview);
    CX_RETURN(root == NULL, NULL);
    cxViewXML xml = root;
    return cxHashGet(xml->items, cxHashStrKey(key));
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

static void cxViewXMLSet(cxAny pview,cxAny cview,xmlTextReaderPtr reader)
{
    cxViewXML this = pview;
    cxChar *id = cxXMLAttr("id");
    CX_RETURN(id == NULL);
    cxHashSet(this->items, cxHashStrKey(id), cview);
    xmlFree(id);
}

cxAny cxViewXMLMakeElement(const xmlChar *temp,xmlTextReaderPtr reader)
{
    cxView cview = NULL;
    if(ELEMENT_IS_TYPE(cxSprite)){
        cview = CX_CREATE(cxSprite);
    }else if(ELEMENT_IS_TYPE(cxViewXML)){
        cxChar *src = cxXMLAttr("src");
        cview = (cxView)cxViewXMLCreate(src);
        xmlFree(src);
    }else if(ELEMENT_IS_TYPE(cxView)){
        cview = CX_CREATE(cxView);
    }else if(ELEMENT_IS_TYPE(cxButton)){
        cview = CX_CREATE(cxButton);
    }else if(ELEMENT_IS_TYPE(cxAtlas)){
        cview = CX_CREATE(cxAtlas);
    }else if(ELEMENT_IS_TYPE(cxParticle)){
        cview = CX_CREATE(cxParticle);
    }else if(ELEMENT_IS_TYPE(cxLabel)){
        cview = CX_CREATE(cxLabel);
    }else if(ELEMENT_IS_TYPE(cxClipping)){
        cview = CX_CREATE(cxClipping);
    }else if(ELEMENT_IS_TYPE(cxTable)){
        cview = CX_CREATE(cxTable);
    }else if(ELEMENT_IS_TYPE(cxScroll)){
        cview = CX_CREATE(cxScroll);
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
            cxAny parent = cxStackTop(stack);
            if(parent == NULL){
                CX_ERROR("parse xml ui parent null");
                break;
            }
            const xmlChar *temp = xmlTextReaderConstName(reader);
            if(temp == NULL){
                continue;
            }
            cxView cview = CX_METHOD_GET(NULL, this->Make, temp, reader);
            cxBool save = false;
            if(cview != NULL){
                cxViewAppend(parent, cview);
                //set root xmlview
                cxObjectSetRoot(cview, this);
                //read attr
                save = cxObjectXMLReadRun(cview, this, reader);
            }
            if(save){
                cxViewXMLSet(this, cview, reader);
            }
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
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(ELEMENT_IS_TYPE(cxViewXML)){
            ret = true;
            break;
        }
    }
    if(ret){
        cxAutoPoolPush();
        cxObjectXMLReadRun(xmlView, xmlView, reader);
        cxStackPush(stack, xmlView);
        CX_EVENT_FIRE(xmlView, onLoad);
        cxViewXMLLoadSubviews(xmlView,reader, stack);
        cxStackPop(stack);
        cxAutoPoolPop();
    }
    CX_RELEASE(stack);
    return ret;
}

cxBool cxViewXMLLoad(cxAny pview,cxConstChars xml)
{
    cxXMLScript script = cxEngineGetXMLScript(xml);
    if(script == NULL || script->bytes == NULL){
        CX_ERROR("%s script not register",xml);
        return false;
    }
    cxBool ret = false;
    xmlTextReaderPtr reader = cxXMLReaderForScript(script);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return false;
    }
    xmlTextReaderSetErrorHandler(reader, cxViewXMLReaderError, pview);
    ret = cxViewXMLLoadWithReader(pview, reader);
    xmlFreeTextReader(reader);
    return ret;
}











