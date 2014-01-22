//
//  cxActionRoot.c
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
#include <actions/cxSpline.h>
#include <actions/cxRunParticle.h>
#include <actions/cxActionSet.h>
#include <actions/cxParabola.h>
#include "cxViewRoot.h"
#include "cxActionRoot.h"
#include "cxPlayer.h"

void cxActionRootReadAttr(cxReaderAttrInfo *info)
{
    cxObjectReadAttr(info);
    cxActionRoot this = info->root;
    cxXMLAppendEvent(info, this, cxActionSet, onLoad);
}

CX_OBJECT_INIT(cxActionRoot, cxObject)
{
    cxObjectSetReadAttrFunc(this, cxActionRootReadAttr);
    this->codes = CX_ALLOC(cxHash);
    CX_METHOD_OVERRIDE(this->Make, cxActionRootMakeElement);
}
CX_OBJECT_FREE(cxActionRoot, cxObject)
{
    CX_EVENT_RELEASE(this->onLoad);
    CX_RELEASE(this->codes);
    CX_METHOD_RELEASE(this->Make);
}
CX_OBJECT_TERM(cxActionRoot, cxObject)

static void cxActionRootReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxActionRoot this = arg;CX_ERROR("%s",msg);this->isError = true;
}

cxAny cxActionRootMakeElement(cxConstChars temp,xmlTextReaderPtr reader)
{
    cxEngine engine = cxEngineInstance();
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
    }else if(ELEMENT_IS_TYPE(cxSpline)){
        action = CX_CREATE(cxSpline);
    }else if(ELEMENT_IS_TYPE(cxRunParticle)){
        action = CX_CREATE(cxRunParticle);
    }else if(ELEMENT_IS_TYPE(cxParabola)){
        action = CX_CREATE(cxParabola);
    }else{
        action = CX_METHOD_FIRE(NULL, engine->MakeAction, temp, reader);
    }
    return action;
}

cxAny cxActionRootGet(cxConstChars src)
{
    cxUrlPath path = cxUrlPathParse(src);
    CX_RETURN(path == NULL, NULL);
    cxActionRoot this = cxActionRootCreate(path->path);
    CX_RETURN(this == NULL, NULL);
    cxString code = cxHashGet(this->codes, (path->count == 1) ? cxHashStrKey(path->path) : cxHashStrKey(path->key));
    CX_RETURN(code == NULL, NULL);
    xmlTextReaderPtr reader = cxXMLReaderForString(code, NULL, NULL);
    cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, this, NULL);
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        info->object = CX_METHOD_FIRE(NULL, this->Make,temp,reader);
        if(info->object == NULL){
            continue;
        }
        //save root
        cxObjectSetRoot(info->object, this);
        cxObjectReadAttrRun(info);
    }
    return info->object;
}

cxAction cxActionRootAttachView(cxAny pview,cxConstChars url)
{
    cxAny action = cxActionRootGet(url);
    CX_RETURN(action == NULL, NULL);
    cxViewAppendAction(pview, action);
    return action;
}

static void cxActionRootLoadCodesWithReader(cxAny pav,xmlTextReaderPtr reader)
{
    cxActionRoot this = pav;
    cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, pav, pav);
    while(xmlTextReaderRead(reader) && !this->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(!ELEMENT_IS_TYPE(cxActionRoot)){
            continue;
        }
        cxObjectReadAttrRun(info);
        CX_EVENT_FIRE(this, onLoad);
        break;
    }
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars id = cxXMLAttr(reader, "id");
        if(id == NULL){
            continue;
        }
        cxString code = cxXMLReaderReadOuterXml(reader);
        cxHashSet(this->codes, cxHashStrKey(id), code);
    }
}

cxAny cxActionRootCreate(cxConstChars xml)
{
    cxEngine engine = cxEngineInstance();
    cxActionRoot this = cxHashGet(engine->actions, cxHashStrKey(xml));
    if(this != NULL){
        return this;
    }
    cxXMLScript script = cxEngineGetXMLScript(xml);
    if(script == NULL || script->bytes == NULL){
        CX_ERROR("%s script not register",xml);
        return NULL;
    }
    this = CX_CREATE(cxActionRoot);
    xmlTextReaderPtr reader = cxXMLReaderForScript(script,cxActionRootReaderError, this);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return NULL;
    }
    cxActionRootLoadCodesWithReader(this, reader);
    cxHashKey key = cxHashStrKey(xml);
    cxHashSet(this->codes, key, script->bytes);
    cxHashSet(engine->actions, key, this);
    return this;
}








