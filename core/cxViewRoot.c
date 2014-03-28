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
#include <views/cxLabelTTF.h>
#include <views/cxLabelBMP.h>
#include <views/cxClipping.h>
#include <views/cxTable.h>
#include <views/cxScroll.h>
#include <views/cxAtlasSet.h>
#include <views/cxPolygon.h>
#include "cxViewRoot.h"
#include "cxHashRoot.h"
#include "cxActionRoot.h"

static cxInt cxViewRootLuaGetView(lua_State *L)
{
    CX_LUA_DEF_THIS(cxViewRoot);
    cxConstChars key = luaL_checkstring(L, 2);
    cxAny any = cxViewRootGet(this, key);
    CX_LUA_PUSH_OBJECT(any);
    return 1;
}

static cxInt cxViewRootLuaMake(lua_State *L)
{
    cxConstChars url = luaL_checkstring(L, 1);
    cxAny any = cxViewRootCreate(url);
    CX_LUA_PUSH_OBJECT(any);
    return 1;
}

CX_LUA_METHOD_BEG(cxViewRoot)
    {"GetView",cxViewRootLuaGetView},
    {"Make",cxViewRootLuaMake},
CX_LUA_METHOD_END(cxViewRoot)

void __cxViewRootTypeInit()
{
    CX_LUA_LOAD_TYPE(cxViewRoot);
}

void cxViewRootReadAttr(cxReaderAttrInfo *info)
{
    cxViewReadAttr(info);
    cxViewRoot this = info->object;
    cxXMLAppendEvent(info, this, cxViewRoot, onBegin);
    cxXMLAppendEvent(info, this, cxViewRoot, onEnd);
}

void cxViewRootSetItem(cxAny pview,cxConstChars key,cxAny item)
{
    cxViewRoot this = pview;
    cxHashSet(this->items, cxHashStrKey(key), item);
}

CX_OBJECT_INIT(cxViewRoot, cxView)
{
    cxObjectSetReadAttrFunc(this, cxViewRootReadAttr);
    CX_METHOD_OVERRIDE(this->Make, cxViewRootMakeElement);
    this->items = CX_ALLOC(cxHash);
    this->actions = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxViewRoot, cxView)
{
    CX_EVENT_RELEASE(this->onBegin);
    CX_EVENT_RELEASE(this->onEnd);
    CX_RELEASE(this->items);
    CX_RELEASE(this->actions);
    CX_METHOD_RELEASE(this->Make);
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
    cxHashDel(this->items, cxHashStrKey(key));
    cxViewRemoved(view);
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

void cxViewRootSet(cxReaderAttrInfo *info)
{
    cxViewRoot this = info->root;
    cxConstChars id = cxXMLAttr(info->reader,"id");
    CX_RETURN(id == NULL);
    cxHashSet(this->items, cxHashStrKey(id), info->object);
}

cxAny cxViewRootMakeElement(cxConstChars temp,xmlTextReaderPtr reader)
{
    cxEngine engine = cxEngineInstance();
    cxAny cview = NULL;
    if(ELEMENT_IS_TYPE(cxSprite)){
        cview = CX_CREATE(cxSprite);
    }else if(ELEMENT_IS_TYPE(cxViewRoot)){
        cxConstChars src = cxXMLAttr(reader, "src");
        CX_ASSERT(src != NULL, "require src attr");
        cview = cxViewRootCreate(src);
    }else if(ELEMENT_IS_TYPE(cxView)){
        cview = CX_CREATE(cxView);
    }else if(ELEMENT_IS_TYPE(cxButton)){
        cview = CX_CREATE(cxButton);
    }else if(ELEMENT_IS_TYPE(cxAtlas)){
        cview = CX_CREATE(cxAtlas);
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
    }else if(ELEMENT_IS_TYPE(cxAtlasSet)){
        cview = CX_CREATE(cxAtlasSet);
    }else if(ELEMENT_IS_TYPE(cxPolygon)){
        cview = CX_CREATE(cxPolygon);
    }else{
        cview = CX_METHOD_GET(NULL, engine->MakeView,temp,reader);
    }
    return cview;
}


static void cxViewRootMakeErrorFunc(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    *(cxBool *)arg = true;
    CX_ERROR("%s",msg);
}

cxAny cxViewRootMake(cxString xml)
{
    cxBool error = false;
    xmlTextReaderPtr reader = cxXMLReaderForString(xml, cxViewRootMakeErrorFunc, &error);
    while(xmlTextReaderRead(reader) && !error){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        cxAny any = cxViewRootMakeElement(temp, reader);
        if(any != NULL){
            cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, NULL, any);
            cxObjectReadAttrRun(info);
            return any;
        }
    }
    return NULL;
}

static void cxViewRootLoadSubviews(cxAny pview,xmlTextReaderPtr reader,cxStack stack)
{
    cxViewRoot this = pview;
    cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, this, NULL);
    while(xmlTextReaderRead(reader)){
        int type = xmlTextReaderNodeType(reader);
        if(type == XML_READER_TYPE_ELEMENT){
            
            cxConstChars temp = cxXMLReadElementName(reader);
            info->object = CX_METHOD_GET(NULL, this->Make,temp,info->reader);
            CX_ASSERT(info->object != NULL, "make element failed");
            cxObjectSetRoot(info->object, this);
            
            cxObjectReadAttrRun(info);
            cxViewRootSet(info);
            
            cxView parent = cxStackTop(stack);
            cxViewAppend(parent, info->object);
            
            if(xmlTextReaderIsEmptyElement(reader)){
                continue;
            }
            cxStackPush(stack, info->object);
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
        cxObjectSetRoot(rootView, rootView);
        cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, rootView, rootView);
        cxObjectReadAttrRun(info);
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











