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
    }else{
        CX_ERROR("action xml can't create type %s",temp);
    }
    return action;
}

cxAny cxActionXMLGet(cxConstChars xml,cxConstChars key)
{
    cxActionXML this = cxActionXMLCreate(xml);
    CX_RETURN(this == NULL, NULL);
    
    cxString code = cxHashGet(this->codes, (key == NULL) ? cxHashStrKey(xml) : cxHashStrKey(key));
    CX_RETURN(code == NULL, NULL);
    
    xmlTextReaderPtr reader = cxXMLReaderForString(code);
    cxAny action = NULL;
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        action = CX_METHOD_GET(NULL, this->Make,temp,reader);
        if(action != NULL){
            cxObjectXMLReadRun(action, this, reader);
        }
    }
    xmlTextReaderClose(reader);
    
    return action;
}

void cxPrintMessageEvent(cxAny pview,cxAny arg)
{
    CX_RETURN(arg == NULL);
    cxConstChars msg = cxEventArgToString(arg);
    CX_RETURN(msg == NULL);
    CX_LOGGER("%s",msg);
}

void cxPlaySoundEvent(cxAny object,cxAny arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars src = cxEventArgString(arg, "src");
    CX_ASSERT(src != NULL, "audio src not set");
    cxBool loop = cxEventArgBool(arg, "loop");
    cxPlayFile(src,loop);
}

//need register
void cxViewRunActionEvent(cxAny pview,cxAny arg)
{
    cxView this = pview;
    CX_RETURN(arg == NULL);
    cxConstChars url = cxEventArgString(arg,"src");
    CX_RETURN(url == NULL);
    cxChar file[128];
    cxChar key[128];
    cxInt rv = cxParseURL(url, file, key);
    //get view by id
    cxAny view = pview;
    cxConstChars sview = cxEventArgString(arg,"view");
    if(sview != NULL){
        view = cxViewXMLGet(this, sview);
    }
    CX_RETURN(view == NULL);
    //get action
    cxAny action = NULL;
    if(rv == 2){
        action = cxActionXMLGet(file, key);
    }else if(rv == 1){
        action = cxActionXMLGet(file, NULL);
    }
    CX_RETURN(action == NULL);
    //set action corve
    cxConstChars scurve = cxEventArgString(arg, "curve");
    cxCurveItem curve = cxCurveGet(scurve);
    if(curve != NULL){
        cxActionSetCurve(action, curve->func);
    }
    //append
    cxViewAppendAction(view, action);
}

cxAction cxActionXMLAttachView(cxAny pview,cxConstChars xml,cxConstChars key)
{
    cxAny action = cxActionXMLGet(xml, key);
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
        break;
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








