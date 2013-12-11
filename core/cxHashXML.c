//
//  cxHashXML.c
//  cxEngine
//
//  Created by xuhua on 10/19/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <streams/cxAssetsStream.h>
#include <textures/cxTextureFactory.h>
#include <views/cxAtlas.h>
#include "cxEngine.h"
#include "cxAutoPool.h"
#include "cxStack.h"
#include "cxHashXML.h"
#include "cxDB.h"

cxTypes cxHashXMLReadAtlasBoxPoint(cxHashXML xml,cxConstChars texfile,xmlTextReaderPtr reader)
{
    cxTypes types = cxAtlasBoxPointTypesCreate();
    cxTexture texture = NULL;
    cxInt index = 0;
    if(texfile != NULL){
        types->assist = cxStringAllocChars(texfile);
        texture = cxTextureFactoryLoadFile(texfile);
    }
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(!ELEMENT_IS_TYPE(item)){
            continue;
        }
        cxAtlasBoxPointType item = {0};
        
        cxChar *top = cxXMLAttr("top");
        if(top != NULL){
            item.mask |= cxViewAutoResizeTop;
            item.box.t = atof(top);
        }
        xmlFree(top);
        
        cxChar *left = cxXMLAttr("left");
        if(left != NULL){
            item.mask |= cxViewAutoResizeLeft;
            item.box.l = atof(left);
        }
        xmlFree(left);
        
        cxChar *right = cxXMLAttr("right");
        if(right != NULL){
            item.mask |= cxViewAutoResizeRight;
            item.box.r = atof(right);
        }
        xmlFree(right);
        
        cxChar *bottom = cxXMLAttr("bottom");
        if(bottom != NULL){
            item.mask |= cxViewAutoResizeBottom;
            item.box.b = atof(bottom);
        }
        xmlFree(bottom);
        
        cxChar *fill = cxXMLAttr("fill");
        if(cxConstCharsEqu(fill, "true")){
            item.box = cxBox4fv(0, 0, 0, 0);
            item.mask = cxViewAutoResizeFill;
        }
        xmlFree(fill);
        
        item.pos.x = cxXMLReadFloatAttr(reader, NULL, "x", 0);
        item.pos.y = cxXMLReadFloatAttr(reader, NULL, "y", 0);
        
        item.size.w = cxXMLReadFloatAttr(reader, NULL, "w", 0);
        item.size.h = cxXMLReadFloatAttr(reader, NULL, "h", 0);
        
        item.color = cxXMLReadColor4fAttr(reader, NULL, "color", cxColor4fv(1, 1, 1, 1));
        
        item.texbox = cxBoxTex2fDefault();
        cxChar *skey = cxXMLAttr("key");
        if(skey != NULL){
            item.texbox = cxTextureBox(texture, skey);
            cxTypesSet(types, skey, cxNumberInt(index));
        }
        if(skey != NULL && cxSize2Zero(item.size)){
            item.size = cxTextureSize(texture, skey);
        }
        xmlFree(skey);
        index ++;
        cxTypesAppend(types, item);
    }
    return types;
}

static void cxHashXMLReadDB(cxDBEnv env,cxHashXML xml,xmlTextReaderPtr reader)
{
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(!ELEMENT_IS_TYPE(cxDB)){
            continue;
        }
        cxChar *file = cxXMLAttr("file");
        cxChar *table = cxXMLAttr("table");
        cxChar *type = cxXMLAttr("type");
        cxChar *sid = cxXMLAttr("id");
        cxChar *path = cxXMLAttr("path");
        cxBool rdonly = cxXMLReadBoolAttr(reader, NULL,  "rdonly", false);
        if(sid == NULL){
            CX_WARN("db id not set,will can't add dataset");
        }
        cxString sfile = NULL;
        if(cxConstCharsEqu(path, "assert")){
            sfile = cxAssetsPath(file);
            //assert must set true
            rdonly = true;
        }else if(cxConstCharsEqu(path, "document")){
            sfile = cxDocumentPath(file);
        }else{
            CX_ERROR("must set path assert or document");
        }
        cxAny db = NULL;
        if(file != NULL && table != NULL && type != NULL){
            db = cxDBCreate(env, cxStringBody(sfile), table, type, rdonly);
        }
        if(db != NULL && sid != NULL){
            cxHashSet(xml->items, cxHashStrKey(sid), cxDBTypesCreate(db));
        }else{
            CX_ERROR("open dbenv type %s,db %s:%s failed",cxStringBody(env->type),file,table);
        }
        xmlFree(path);
        xmlFree(sid);
        xmlFree(file);
        xmlFree(table);
        xmlFree(type);
    }
}

void cxHashXMLReadDBEnv(cxHashXML xml,xmlTextReaderPtr reader)
{
    cxChar *type = cxXMLAttr("type");
    cxBool logger = cxXMLReadBoolAttr(reader,NULL, "logger", false);
    cxDBEnv env = cxDBEnvCreate(type, logger);
    if(env != NULL){
        cxHashXMLReadDB(env, xml, reader);
    }
    xmlFree(type);
}

cxTypes cxHashXMLReadString(cxHashXML xml,xmlTextReaderPtr reader)
{
    cxTypes types = cxStringTypesCreate();
    cxString bytes = NULL;
    cxChar *src = cxXMLAttr("src");
    if(src != NULL){
        cxStream stream = cxAssetsStreamCreate(src);
        bytes = cxStreamAllBytes(stream);
    }else if(!xmlTextReaderIsEmptyElement(reader)) {
        bytes = cxXMLReadString(reader);
    }else{
        bytes = NULL;
    }
    xmlFree(src);
    if(bytes == NULL){
        return NULL;
    }
    CX_RETAIN_SWAP(types->assist, bytes);
    return types;
}

/*
 <cxHash id="config">
    <cxString key="name">text</cxString>
    <cxInt key="int">123</cxInt>
    <cxFloat key="float">456</cxInt>
    <cxDouble key="double">789</cxInt>
    <cxVec2f key="vec2f">12,23</cxVec2f>
    <cxVec2i key="vec2i">12,23</cxVec2i>
    <cxSize2f key="size2f">100,200</cxSize2f>
 </cxHash>
*/

static cxAny cxReadValues(cxHashXML xml,const xmlChar *temp,xmlTextReaderPtr reader)
{
    cxNumberValue vm={0};
    cxString text = cxXMLReadString(reader);
    CX_RETURN(text == NULL, NULL);
    cxConstChars value = cxStringBody(text);
    cxAny rv = NULL;
    if(ELEMENT_IS_TYPE(cxString)){
        rv = cxStringConstChars(value);
    }else if(ELEMENT_IS_TYPE(cxInt)){
        rv = cxNumberInt(atoi(value));
    }else if(ELEMENT_IS_TYPE(cxFloat)){
        rv = cxNumberFloat(atof(value));
    }else if (ELEMENT_IS_TYPE(cxDouble)){
        rv = cxNumberDouble(atof(value));
    }else if (ELEMENT_IS_TYPE(cxVec2f) && cxReadFloats(value, &vm.vec2f.x) == 2){
        rv = cxNumberVec2f(vm.vec2f);
    }else if (ELEMENT_IS_TYPE(cxSize2f) && cxReadFloats(value, &vm.size2f.w) == 2){
        rv = cxNumberSize2f(vm.size2f);
    }else if(ELEMENT_IS_TYPE(cxColor4f) && cxReadFloats(value, &vm.color4f.r) == 4){
        rv = cxNumberColor4f(vm.color4f);
    }else if(ELEMENT_IS_TYPE(cxVec2i) && cxReadInts(value, &vm.vec2i.x) == 2){
        rv = cxNumberVec2i(vm.vec2i);
    }else if(ELEMENT_IS_TYPE(cxHash)){
        rv = cxHashXMLReadHash(xml,reader);
    }
    return rv;
}

cxTypes cxHashXMLReadHash(cxHashXML xml,xmlTextReaderPtr reader)
{
    cxTypes types = cxHashTypesCreate();
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxChar *key = cxXMLAttr("key");
        if(key == NULL){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        cxAny value = cxReadValues(xml, temp, reader);
        if(value != NULL){
            cxTypesSet(types, key, value);
        }
        xmlFree(key);
    }
    return types;
}

cxTypes cxHashXMLReadArray(cxHashXML xml,xmlTextReaderPtr reader)
{
    cxTypes types = cxArrayTypesCreate();
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        cxAny value = cxReadValues(xml, temp, reader);
        if(value != NULL){
            cxArrayAppend(types->assist, value);
        }
    }
    return types;
}

CX_OBJECT_INIT(cxHashXML, cxObject)
{
    this->items = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxHashXML, cxObject)
{
    CX_RELEASE(this->items);
}
CX_OBJECT_TERM(cxHashXML, cxObject)

static void cxHashXMLReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxHashXML this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

cxHashXML cxHashXMLCreate(cxConstChars xml)
{
    cxHashXML this = CX_CREATE(cxHashXML);
    if(!cxHashXMLLoad(this, xml)){
        return NULL;
    }
    return this;
}

cxBool cxHashXMLLoad(cxAny hashxml,cxConstChars xml)
{
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
    xmlTextReaderSetErrorHandler(reader, cxHashXMLReaderError, hashxml);
    ret = cxHashXMLLoadWithReader(hashxml, reader);
    xmlFreeTextReader(reader);
    return ret;
}

cxBool cxHashXMLLoadWithReader(cxAny hash,xmlTextReaderPtr reader)
{
    cxBool ret = false;
    cxHashXML xml = hash;
    while(xmlTextReaderRead(reader)){
        if(xml->isError){
            break;
        }
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(ELEMENT_IS_TYPE(cxHashXML)){
            ret = true;
            break;
        }
    }
    if(!ret){
        return false;
    }
    cxAutoPoolPush();
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT){
            const xmlChar *temp = xmlTextReaderConstName(reader);
            
            if(ELEMENT_IS_TYPE(cxDBEnv)){
                cxHashXMLReadDBEnv(xml,reader);
                continue;
            }
            cxChar *sid = cxXMLAttr("id");
            if(sid == NULL){
                CX_ERROR("must set id");
                continue;
            }
            cxAny object = NULL;
            if(ELEMENT_IS_TYPE(cxAtlasBoxPoint)){
                cxChar *stexture = cxXMLAttr("texture");
                object = cxHashXMLReadAtlasBoxPoint(xml,stexture,reader);
                xmlFree(stexture);
            }else if(ELEMENT_IS_TYPE(cxString)){
                object = cxHashXMLReadString(xml,reader);
            }else if(ELEMENT_IS_TYPE(cxHash)){
                object = cxHashXMLReadHash(xml,reader);
            }else if(ELEMENT_IS_TYPE(cxArray)){
                object = cxHashXMLReadArray(xml,reader);
            }else{
                object = cxReadValues(xml, temp, reader);
            }
            if(object != NULL){
                cxHashSet(xml->items, cxHashStrKey(sid), object);
            }
            xmlFree(sid);
        }
    }
    cxAutoPoolPop();
    return ret;
}











