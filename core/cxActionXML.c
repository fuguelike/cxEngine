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
#include "cxViewXML.h"
#include "cxActionXML.h"
#include "cxPlayer.h"

static void cxActionRootXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXMLReadAttr(xmlAction, mAction, reader);
    cxActionXML xml = xmlAction;
    cxActionXML this = mAction;
    //type multiple sequence
    cxChar *stype = cxXMLAttr("cxActionXML.type");
    if(strcasecmp(stype, "multiple") == 0){
        xml->type = cxActionXMLTypeMultiple;
    }else if(strcasecmp(stype, "sequence") == 0){
        xml->type = cxActionXMLTypeSequence;
    }else{
        xml->type = cxActionXMLTypeNone;
    }
    xmlFree(stype);
    //
    cxXMLAppendEvent(xml->events, this, cxActionXML, onLoad);
}

static void cxActionItemStop(cxAny pav,cxAny arg)
{
    cxActionXML xml = cxEventArgToWeakRef(arg);
    CX_ASSERT(xml != NULL, "event arg not set");
    xml->index ++;
    if(xml->type == cxActionXMLTypeSequence){
        cxActionXMLRunNext(xml);
    }
}

void cxActionXMLRunNext(cxAny pav)
{
    cxActionXML this = pav;
    if(this->index >= 0 && this->index < cxArrayLength(this->items)){
        cxAction action = cxArrayAtIndex(this->items, this->index);
        CX_EVENT_APPEND(action->onStop, cxActionItemStop, cxEventArgWeakRef(this));
        cxViewAppendAction(this->super.view, action);
    }
}

void cxActionXMLRunAll(cxAny pav)
{
    cxActionXML this = pav;
    CX_ARRAY_FOREACH(this->items, ele){
        cxAction action = cxArrayObject(ele);
        CX_EVENT_APPEND(action->onStop, cxActionItemStop, cxEventArgWeakRef(this));
        cxViewAppendAction(this->super.view, action);
    }
}

static void cxActionXMLInit(cxAny pav)
{
    cxActionXML this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    CX_HASH_FOREACH(this->actions, ele, tmp){
        cxAction action = ele->any;
        cxArrayAppend(this->items, action);
    }
    if(this->type == cxActionXMLTypeSequence){
        this->index = 0;
        cxActionXMLRunNext(this);
        return;
    }
    if(this->type == cxActionXMLTypeMultiple){
        this->index = 0;
        cxActionXMLRunAll(this);
        return;
    }
}

static cxBool cxActionXMLExit(cxAny pav)
{
    cxActionXML this = pav;
    if(this->index >= cxArrayLength(this->items)){
        cxArrayClean(this->items);
        return true;
    }
    return false;
}

static void cxActionXMLStep(cxAny pav,cxFloat dt,cxFloat time)
{
//    cxActionXML this = pav;
//    CX_LOGGER("%f %f",this->super.duration,time);
}

CX_OBJECT_INIT(cxActionXML, cxAction)
{
    CX_METHOD_SET(this->super.Init, cxActionXMLInit);
    CX_METHOD_SET(this->super.Step, cxActionXMLStep);
    CX_METHOD_SET(this->super.Exit, cxActionXMLExit);
    CX_METHOD_SET(this->Make, cxActionXMLMakeElement);
    cxObjectSetXMLReadFunc(this, cxActionRootXMLReadAttr);
    this->actions = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->items = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxActionXML, cxAction)
{
    CX_RELEASE(this->items);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->events);
    CX_EVENT_RELEASE(this->onLoad);
}
CX_OBJECT_TERM(cxActionXML, cxAction)

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
    cxHashSet(this->actions, cxHashStrKey(id), mAction);
    cxObjectSetRoot(mAction,this);
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
    }else{
        CX_ERROR("action xml can't create type %s",temp);
    }
    return action;
}

static void cxActionXMLLoadActions(cxActionXML this,xmlTextReaderPtr reader)
{
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(temp == NULL){
            continue;
        }
        cxAction action = CX_METHOD_GET(NULL, this->Make,temp,reader);
        if(action == NULL){
            CX_ERROR("load actions null");
            continue;
        }
        cxObjectXMLReadRun(action, this, action, reader);
        cxActionXMLSet(this, action, reader);
    }
}

void cxPrintMessageEvent(cxAny pview,cxAny arg)
{
    CX_RETURN(arg == NULL);
    cxString msg = cxEventArgToString(arg);
    CX_RETURN(msg == NULL);
    CX_LOGGER("%s",cxStringBody(msg));
}

void cxPlaySoundEvent(cxAny object,cxAny arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxString src = cxEventArgString(arg, "src");
    CX_ASSERT(src != NULL, "audio src not set");
    cxConstChars path = cxStringBody(src);
    cxBool loop = cxEventArgBool(arg, "loop");
    cxPlayFile(path,loop);
}

//need register
void cxViewRunActionEvent(cxAny pview,cxAny arg)
{
    cxView this = pview;
    CX_RETURN(arg == NULL);
    cxString url = cxEventArgString(arg,"src");
    CX_RETURN(url == NULL);
    cxChar file[128];
    cxChar key[128];
    cxInt rv = cxParseURL(url, file, key);
    //get view by id
    cxAny view = pview;
    cxString sview = cxEventArgString(arg,"view");
    if(sview != NULL){
        view = cxViewXMLGet(this, cxStringBody(sview));
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
    //get corve
    cxString scurve = cxEventArgString(arg, "curve");
    if(scurve != NULL){
        cxCurveItem curve = cxEngineGetCurve(cxStringBody(scurve));
        cxActionSetCurve(action, curve != NULL ? curve->func : NULL);
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
}

cxBool cxActionXMLLoadWithReader(cxAny pav,xmlTextReaderPtr reader)
{
    cxActionXML this = pav;
    cxBool ret = false;
    while(xmlTextReaderRead(reader)){
        if(this->isError){
            break;
        }
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(ELEMENT_IS_TYPE(cxActionXML)){
            ret = true;
            break;
        }
    }
    if(ret){
        cxAutoPoolPush();
        cxObjectXMLReadRun(this, this, this, reader);
        CX_EVENT_FIRE(this, onLoad);
        cxActionXMLLoadActions(this, reader);
        cxAutoPoolPop();
    }
    return ret;
}

cxBool cxActionXMLLoad(cxAny pav,cxConstChars xml)
{
    cxActionXML this = pav;
    cxXMLScript script = cxEngineGetXMLScript(xml);
    if(script == NULL){
        CX_ERROR("%s script not register",xml);
        return false;
    }
    if(script->bytes == NULL){
        CX_ERROR("xml script not load bytes");
        return false;
    }
    cxBool ret = false;
    xmlTextReaderPtr reader = xmlReaderForMemory(cxStringBody(script->bytes), cxStringLength(script->bytes), NULL, "UTF-8", 0);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return false;
    }
    xmlTextReaderSetErrorHandler(reader, cxActionXMLReaderError, this);
    ret = cxActionXMLLoadWithReader(this, reader);
    xmlFreeTextReader(reader);
    return ret;
}

cxAny cxActionXMLCreate(cxConstChars xml)
{
    cxActionXML this = CX_CREATE(cxActionXML);
    if(!cxActionXMLLoad(this, xml)){
        return NULL;
    }
    return this;
}

cxAny cxActionXMLGet(cxConstChars xml,cxConstChars name)
{
    cxActionXML this = cxActionXMLCreate(xml);
    CX_RETURN(name == NULL, this);
    return cxHashGet(this->actions, cxHashStrKey(name));
}







