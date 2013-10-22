//
//  cxActionXML.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxAutoPool.h>
#include "cxViewXML.h"
#include "cxActionXML.h"
#include "cxMove.h"
#include "cxScale.h"
#include "cxRotate.h"

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

static void cxActionItemAfter(cxAny pav,cxAny arg)
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
        CX_EVENT_APPEND(action->onAfter, cxActionItemAfter, cxEventArgWeakRef(this));
        cxViewAppendAction(this->super.view, action);
    }
}

void cxActionXMLRunAll(cxAny pav)
{
    cxActionXML this = pav;
    CX_ARRAY_FOREACH(this->items, ele){
        cxAction action = cxArrayObject(ele);
        CX_EVENT_APPEND(action->onAfter, cxActionItemAfter, cxEventArgWeakRef(this));
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
    cxObjectSetXMLReadFunc(this, cxActionRootXMLReadAttr);
    CX_METHOD_SET(this->super.Exit, cxActionXMLExit);
    this->actions = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->items = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxActionXML, cxAction)
{
    CX_RELEASE(this->items);
    CX_EVENT_RELEASE(this->onLoad);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->events);
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
    cxConstChars id = (cxConstChars)cxXMLAttr("id");
    CX_RETURN(id == NULL);
    cxHashKey key = cxHashStrKey(id);
    CX_ASSERT(cxHashGet(this->actions, key) == NULL, "reapeat action id");
    cxHashSet(this->actions, key, mAction);
    cxObjectSetRoot(mAction,this);
}

static void cxActionXMLLoadActions(cxActionXML this,xmlTextReaderPtr reader)
{
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        cxAction action = NULL;
        if(ELEMENT_IS_TYPE(cxMove)){
            action = (cxAction)CX_CREATE(cxMove);
        }else if(ELEMENT_IS_TYPE(cxScale)){
            action = (cxAction)CX_CREATE(cxScale);
        }else if(ELEMENT_IS_TYPE(cxRotate)){
            action = (cxAction)CX_CREATE(cxRotate);
        }else{
            action = CX_METHOD_GET(NULL, this->Make,(cxConstChars)temp,reader);
        }
        if(action == NULL){
            CX_ERROR("load actions null");
            continue;
        }
        cxObjectSetXMLReadRun(action, this, action, reader);
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
    CX_RETURN(arg == NULL);
    //wav effect
    //mp3 ogg music
    cxString file = cxEventArgString(arg, "file");
    if(file == NULL){
        CX_ERROR("cxPlay file null");
        return;
    }
    cxConstChars path = cxStringBody(file);
    cxBool loop = cxEventArgBool(arg, "loop");
    CX_LOGGER("file=%s loop=%d",path,loop);
}

void cxActionRemoveViewEvent(cxAny pav,cxAny arg)
{
    cxAction this = pav;
    CX_RETURN(this->view == NULL);
    cxViewRemoved(this->view);
}

void cxActionRunActionEvent(cxAny pav,cxAny arg)
{
    cxAction this = pav;
    cxViewRunActionEvent(this->view, arg);
}


//need register
void cxViewRunActionEvent(cxAny pview,cxAny arg)
{
    cxView this = pview;
    CX_RETURN(arg == NULL);
    cxString url = cxEventArgString(arg,"file");
    CX_RETURN(url == NULL);
    cxChar file[128];
    cxChar key[128];
    cxInt rv = cxParseURL(url, file, key);
    CX_RETURN(rv == 0);
    cxAny target = pview;
    cxString starget = cxEventArgString(arg,"target");
    if(starget != NULL){
        target = cxViewXMLGet(this, cxStringBody(starget));
    }
    CX_RETURN(target == NULL);
    cxActionXMLAttachView(target, file, rv == 2 ? key : NULL);
}

cxAction cxActionXMLAttachView(cxAny pview,cxConstChars xml,cxConstChars key)
{
    cxAny action = NULL;
    if(key == NULL){
        action = cxActionXMLCreate(xml);
    }else{
        action = cxActionXMLClone(xml, key);
    }
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
        cxObjectSetXMLReadRun(this, this, this, reader);
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

cxAny cxActionXMLClone(cxConstChars xml,cxConstChars name)
{
    cxActionXML xmlAction = cxEngineLoadActionXML(xml);
    CX_RETURN(xmlAction == NULL,NULL);
    cxAction acton = cxHashGet(xmlAction->actions, cxHashStrKey(name));
    CX_RETURN(acton == NULL,NULL);
    return cxActionClone(acton);
}







