//
//  cxActionXML.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxAutoPool.h>
#include <actions/cxMove.h>
#include <actions/cxScale.h>
#include <actions/cxRotate.h>
#include <actions/cxJump.h>
#include <actions/cxFade.h>
#include <actions/cxTint.h>
#include <actions/cxTimer.h>
#include <actions/cxAnimate.h>
#include <actions/cxActionSet.h>
#include "cxViewXML.h"
#include "cxActionXML.h"
#include "cxPlayer.h"

cxBool cxActionRootXMLReadAttr(cxAny pxml,cxAny newobj, xmlTextReaderPtr reader)
{
    cxObjectXMLReadAttr(pxml, newobj, reader);
    cxActionXML this = pxml;
    cxXMLAppendEvent(this->events, this, cxActionSet, onLoad);
    return true;
}

CX_OBJECT_INIT(cxActionXML, cxObject)
{
    cxObjectSetXMLReadFunc(this, cxActionRootXMLReadAttr);
    this->codes = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    CX_METHOD_SET(this->Make, cxActionXMLMakeElement);
}
CX_OBJECT_FREE(cxActionXML, cxObject)
{
    CX_EVENT_RELEASE(this->onLoad);
    CX_RELEASE(this->events);
    CX_RELEASE(this->codes);
}
CX_OBJECT_TERM(cxActionXML, cxObject)

static void cxActionXMLReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxActionXML this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

void cxActionXMLSet(cxAny xmlAction,cxAny mAction,xmlTextReaderPtr reader)
{
    cxActionXML this = xmlAction;
    cxChar *id = cxXMLAttr("id");
    CX_RETURN(id == NULL);
    cxString code = cxXMLReaderReadOuterXml(reader);
    cxHashSet(this->codes, cxHashStrKey(id), code);
    xmlFree(id);
}

cxAny cxActionXMLMakeElement(const xmlChar *temp,xmlTextReaderPtr reader)
{
    cxAny action = NULL;
    if(ELEMENT_IS_TYPE(cxMove)){
        action = CX_CREATE(cxMove);
    }else if(ELEMENT_IS_TYPE(cxScale)){
        action = CX_CREATE(cxScale);
    }else if(ELEMENT_IS_TYPE(cxRotate)){
        action = CX_CREATE(cxRotate);
    }else if(ELEMENT_IS_TYPE(cxJump)){
        action = CX_CREATE(cxJump);
    }else if(ELEMENT_IS_TYPE(cxFade)){
        action = CX_CREATE(cxFade);
    }else if(ELEMENT_IS_TYPE(cxTint)){
        action = CX_CREATE(cxTint);
    }else if(ELEMENT_IS_TYPE(cxTimer)){
        action = CX_CREATE(cxTimer);
    }else if(ELEMENT_IS_TYPE(cxActionSet)){
        action = CX_CREATE(cxActionSet);
    }else if(ELEMENT_IS_TYPE(cxAnimate)){
        action = CX_CREATE(cxAnimate);
    }else{
        CX_ERROR("action xml can't create type %s",temp);
    }
    return action;
}

cxAny cxActionXMLGet(cxConstChars src)
{
    cxChar file[128]={0};
    cxChar key[128]={0};
    cxInt rv = cxParseURL(src, file, key);
    CX_RETURN(rv == 0, NULL);
    cxActionXML this = cxActionXMLCreate(file);
    CX_RETURN(this == NULL, NULL);
    cxString code = cxHashGet(this->codes, (rv == 1) ? cxHashStrKey(file) : cxHashStrKey(key));
    CX_RETURN(code == NULL, NULL);
    xmlTextReaderPtr reader = cxXMLReaderForString(code);
    cxAny action = NULL;
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        action = CX_METHOD_GET(NULL, this->Make,temp,reader);
        if(action == NULL){
            continue;
        }
        cxObjectXMLReadRun(action, this, reader);
    }
    xmlTextReaderClose(reader);
    return action;
}

void cxPrintMessageEvent(cxEvent *event)
{
    CX_RETURN(event->args == NULL);
    cxConstChars msg = cxEventArgToString(event->args);
    CX_RETURN(msg == NULL);
    CX_LOGGER("%s",msg);
}

void cxPlaySoundEvent(cxEvent *event)
{
    CX_ASSERT(event->args != NULL, "args error");
    cxConstChars src = cxEventArgString(event->args, "src");
    CX_ASSERT(src != NULL, "audio src not set");
    cxBool loop = cxEventArgBool(event->args, "loop");
    cxPlayFile(src,loop);
}

//need register
//src view curve delay
void cxViewRunActionEvent(cxEvent *event)
{
    cxObject object = event->sender;
    CX_RETURN(event->args == NULL);
    cxConstChars src = cxEventArgString(event->args,"src");
    CX_RETURN(src == NULL);
    //get view by id and cxBase
    cxAny view = NULL;
    if(object->cxBase == cxBaseTypeAction){
        view = cxActionView(object);
    }else if(object->cxBase == cxBaseTypeView){
        view = object;
    }
    CX_ASSERT(view != NULL, "this event's sender must base cxAction and cxView");
    //
    cxConstChars sview = cxEventArgString(event->args,"view");
    if(sview != NULL){
        view = cxViewXMLGet(view, sview);
        CX_RETURN(view == NULL);
    }
    //get action
    cxAny action = NULL;
    cxBool fromcache =  false;
    cxBool cache = cxEventArgBool(event->args, "cache");
    if(cache){
        action = cxViewGetCache(view, src);
        fromcache = (action != NULL);
    }
    if(action == NULL){
        action = cxActionXMLGet(src);
    }
    CX_RETURN(action == NULL);
    if(!fromcache && cache){
        cxViewSetCache(view, src, action);
    }
    if(fromcache){
        cxActionReset(action);
    }
    //set action corve
    cxConstChars scurve = cxEventArgString(event->args, "curve");
    cxCurveItem curve = cxCurveGet(scurve);
    if(curve != NULL){
        cxActionSetCurve(action, curve->func);
    }
    //delay
    cxFloat delay = cxEventArgDouble(event->args, "delay");
    if(delay > 0){
        cxActionSetDelay(action, delay);
    }
    //append
    cxViewAppendAction(view, action);
}

cxAction cxActionXMLAttachView(cxAny pview,cxConstChars url)
{
    cxAny action = cxActionXMLGet(url);
    CX_RETURN(action == NULL, NULL);
    cxViewAppendAction(pview, action);
    return action;
    return NULL;
}

static void cxActionXMLLoadCodesWithReader(cxAny pav,xmlTextReaderPtr reader)
{
    cxActionXML this = pav;
    while(xmlTextReaderRead(reader) && !this->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(!ELEMENT_IS_TYPE(cxActionSet)){
            continue;
        }
        cxObjectXMLReadRun(this, this, reader);
        CX_EVENT_FIRE(this, onLoad);
        break;
    }
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxChar *id = cxXMLAttr("id");
        if(id == NULL){
            continue;
        }
        cxString code = cxXMLReaderReadOuterXml(reader);
        cxHashSet(this->codes, cxHashStrKey(id), code);
        xmlFree(id);
    }
}

cxAny cxActionXMLCreate(cxConstChars xml)
{
    cxEngine engine = cxEngineInstance();
    cxActionXML this = cxHashGet(engine->actions, cxHashStrKey(xml));
    if(this != NULL){
        return this;
    }
    cxXMLScript script = cxEngineGetXMLScript(xml);
    if(script == NULL || script->bytes == NULL){
        CX_ERROR("%s script not register",xml);
        return NULL;
    }
    xmlTextReaderPtr reader = cxXMLReaderForScript(script);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return NULL;
    }
    this = CX_CREATE(cxActionXML);
    xmlTextReaderSetErrorHandler(reader, cxActionXMLReaderError, this);
    cxActionXMLLoadCodesWithReader(this, reader);
    xmlFreeTextReader(reader);
    cxHashKey key = cxHashStrKey(xml);
    cxHashSet(this->codes, key, script->bytes);
    cxHashSet(engine->actions, key, this);
    return this;
}








