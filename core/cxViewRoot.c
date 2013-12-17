//
//  cxViewRoot.c
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
#include "cxViewRoot.h"
#include "cxHashRoot.h"
#include "cxActionRoot.h"

void cxViewRootReadAttr(cxAny pxml,cxAny view, xmlTextReaderPtr reader)
{
    cxViewReadAttr(pxml, view, reader);
    cxViewRoot this = view;
    cxViewRoot root = pxml;
    cxXMLAppendEvent(root->events, this, cxViewRoot, onBegin);
    cxXMLAppendEvent(root->events, this, cxViewRoot, onEnd);
}

void cxViewRootRegisteEvent(cxAny pview,cxConstChars name,cxEventFunc func)
{
    cxViewRoot this = pview;
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

void cxViewRootRegisteFunc(cxAny pview,cxConstChars name,cxAny func)
{
    cxViewRoot this = pview;
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

void cxViewRootSetItem(cxAny pview,cxConstChars key,cxAny item)
{
    cxViewRoot this = pview;
    cxHashSet(this->items, cxHashStrKey(key), item);
}

CX_OBJECT_INIT(cxViewRoot, cxView)
{
    cxObjectSetReadAttrFunc(this, cxViewRootReadAttr);
    CX_METHOD_SET(this->Make, cxViewRootMakeElement);
    this->items = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->actions = CX_ALLOC(cxHash);
    this->functions = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxViewRoot, cxView)
{
    CX_EVENT_RELEASE(this->onBegin);
    CX_EVENT_RELEASE(this->onEnd);
    CX_RELEASE(this->functions);
    CX_RELEASE(this->items);
    CX_RELEASE(this->events);
    CX_RELEASE(this->actions);
}
CX_OBJECT_TERM(cxViewRoot, cxView)

cxAny cxViewRootGet(cxAny pview,cxConstChars key)
{
    cxViewRoot root = cxObjectRoot(pview);
    return (root != NULL) ? cxHashGet(root->items, cxHashStrKey(key)) : NULL;
}

void cxViewRootRemove(cxAny pview,cxConstChars key)
{
    cxViewRoot this = pview;
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
    cxViewRoot view = cxViewRootCreate(url);
    CX_ASSERT(view != NULL, "create xml view %s falied ",url);
    cxWindowReplaceView(view,event->args);
}

void cxViewPushViewEvent(cxEvent *event)
{
    CX_ASSERT(event->args != NULL, "args error");
    cxConstChars url = cxEventArgToString(event->args);
    CX_ASSERT(url != NULL, "args error");
    cxViewRoot view = cxViewRootCreate(url);
    CX_ASSERT(view != NULL, "create xml view %s falied ",url);
    cxWindowPushView(view,event->args);
}

void cxViewPopViewEvent(cxEvent *event)
{
    cxWindowPopView(event->args);
}

cxViewRoot cxViewRootCreate(cxConstChars xml)
{
    cxViewRoot this = CX_CREATE(cxViewRoot);
    if(!cxViewRootLoad(this, xml)){
        return NULL;
    }
    cxObjectSetRoot(this, this);
    return this;
}

void cxViewRootSet(cxAny pview,cxAny cview,xmlTextReaderPtr reader)
{
    cxViewRoot this = pview;
    cxConstChars id = cxXMLAttr("id");
    CX_RETURN(id == NULL);
    cxHashSet(this->items, cxHashStrKey(id), cview);
}

cxAny cxViewRootMakeElement(cxConstChars temp,xmlTextReaderPtr reader)
{
    cxAny cview = NULL;
    if(ELEMENT_IS_TYPE(cxSprite)){
        cview = CX_CREATE(cxSprite);
    }else if(ELEMENT_IS_TYPE(cxViewRoot)){
        cxConstChars src = cxXMLAttr("src");
        CX_ASSERT(src != NULL, "require src attr");
        cview = cxViewRootCreate(src);
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

cxAny cxViewRootLoadSubviewBegin(cxViewRoot root,cxConstChars temp,xmlTextReaderPtr reader)
{
    cxView cview = CX_METHOD_GET(NULL, root->Make, temp, reader);
    CX_ASSERT(cview != NULL, "make element null");
    //save root
    cxObjectSetRoot(cview, root);
    //read attr
    cxObjectReadAttrRun(cview, root, reader);
    //save cview -> root
    cxViewRootSet(root, cview, reader);
    return cview;
}

static void cxViewRootLoadSubviews(cxAny pview,xmlTextReaderPtr reader,cxStack stack)
{
    cxViewRoot this = pview;
    while(xmlTextReaderRead(reader)){
        int type = xmlTextReaderNodeType(reader);
        if(type == XML_READER_TYPE_ELEMENT){
            cxConstChars temp = cxXMLReadElementName(reader);
            CX_ASSERT(temp != NULL, "temp read error");
            
            cxAny cview = cxViewRootLoadSubviewBegin(this, temp, reader);
            cxView parent = cxStackTop(stack);
            cxViewAppend(parent, cview);
            
            if(xmlTextReaderIsEmptyElement(reader)){
                continue;
            }
            cxStackPush(stack, cview);
        }else if(type == XML_READER_TYPE_END_ELEMENT){
            cxStackPop(stack);
        }
    }
}

static void cxViewRootReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxViewRoot this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

cxBool cxViewRootLoadWithReader(cxAny pview,xmlTextReaderPtr reader)
{
    cxBool ret = false;
    cxViewRoot rootView = pview;
    cxStack stack = CX_ALLOC(cxStack);
    while(xmlTextReaderRead(reader) && !rootView->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(ELEMENT_IS_TYPE(cxViewRoot)){
            ret = true;
            break;
        }
    }
    if(ret){
        cxAutoPoolPush();
        cxObjectReadAttrRun(rootView, rootView, reader);
        cxStackPush(stack, rootView);
        CX_EVENT_FIRE(rootView, onBegin);
        cxViewRootLoadSubviews(rootView,reader, stack);
        CX_EVENT_FIRE(rootView, onEnd);
        cxStackPop(stack);
        cxAutoPoolPop();
    }
    CX_RELEASE(stack);
    return ret;
}

cxBool cxViewRootLoad(cxAny pview,cxConstChars xml)
{
    cxBool ret = false;
    cxXMLScript script = cxEngineGetXMLScript(xml);
    if(script == NULL || script->bytes == NULL){
        CX_ERROR("%s script not register",xml);
        return ret;
    }
    xmlTextReaderPtr reader = cxXMLReaderForScript(script,cxViewRootReaderError,pview);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return ret;
    }
    ret = cxViewRootLoadWithReader(pview, reader);
    return ret;
}











