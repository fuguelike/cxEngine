//
//  cxXMLScript.h
//  cxEngine
//
//  Created by xuhua on 10/14/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxXMLScript_h
#define cxEngine_cxXMLScript_h

#include <libxml/xmlreader.h>
#include "cxBase.h"
#include "cxString.h"
#include "cxEventItem.h"
#include "cxHash.h"

CX_C_BEGIN

typedef cxAny (*cxXMLScriptMakeElementFunc)(const xmlChar *temp,xmlTextReaderPtr reader);

typedef cxBool (*cxXMLScriptLoadFunc)(cxAny pview);

CX_OBJECT_DEF(cxXMLScript, cxObject)
    cxString bytes;
CX_OBJECT_END(cxXMLScript)

xmlTextReaderPtr cxXMLReaderForString(cxString code);

xmlTextReaderPtr cxXMLReaderForScript(cxXMLScript this);

cxInt cxXMLReadFloatsAttr(xmlTextReaderPtr reader,cxConstChars name,cxFloat *values);

cxEventItem cxXMLReadEvent(cxHash events,cxConstChars name,xmlTextReaderPtr reader);

#define ELEMENT_IS_TYPE(t)  (temp != NULL && xmlStrcmp(temp, BAD_CAST#t) == 0)

#define cxXMLAppendEvent(s,o,t,e)                               \
do{                                                             \
    cxEventItem item = cxXMLReadEvent(s, #t"."#e, reader);      \
    if(item != NULL){                                           \
        CX_EVENT_APPEND(o->e, item->func,item->arg);            \
    }                                                           \
}while(0)

cxChar *cxXMLAttrAuto(xmlTextReaderPtr reader,cxConstChars name);
#define cxXMLAttr(n)    cxXMLAttrAuto(reader,n)

cxInt cxReadFloats(cxConstChars ptr,cxFloat *values);

cxInt cxReadInts(cxConstChars ptr,cxInt *values);

//return need xmlFree free
cxChar *cxXMLReadString(xmlTextReaderPtr reader);

cxFloat cxXMLReadFloatAttr(xmlTextReaderPtr reader,cxConstChars name,cxFloat value);

cxColor4f cxXMLReadColorAttr(xmlTextReaderPtr reader,cxConstChars name,cxColor4f color);

cxString cxXMLReaderReadOuterXml(xmlTextReaderPtr reader);

cxString cxXMLReadLangStringAttr(xmlTextReaderPtr reader,cxConstChars name);

cxInt cxXMLReadIntAttr(xmlTextReaderPtr reader,cxConstChars name,cxInt value);

cxInt cxXMLReadIntsAttr(xmlTextReaderPtr reader,cxConstChars name,cxInt *values);

cxBool cxXMLReadBoolAttr(xmlTextReaderPtr reader,cxConstChars name,cxBool value);

cxVec3f cxXMLReadVec3fAttr(xmlTextReaderPtr reader,cxConstChars name,cxVec3f value);

cxVec2f cxXMLReadVec2fAttr(xmlTextReaderPtr reader,cxConstChars name,cxVec2f value);

CX_C_END

#endif
