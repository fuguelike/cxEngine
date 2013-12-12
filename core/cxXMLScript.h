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
#include "cxTexture.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxTextureAttr, cxObject)
    cxTexture texture;
    cxBoxTex2f box;
    cxSize2f size;
CX_OBJECT_END(cxTextureAttr)

CX_OBJECT_DEF(cxXMLReader, cxObject)
    xmlTextReaderPtr reader;
CX_OBJECT_END(cxXMLReader)

typedef cxAny (*cxXMLScriptMakeElementFunc)(cxConstChars temp,xmlTextReaderPtr reader);

typedef cxBool (*cxXMLScriptLoadFunc)(cxAny pview);

CX_OBJECT_DEF(cxXMLScript, cxObject)
    cxString bytes;
CX_OBJECT_END(cxXMLScript)

cxString cxXMLReaderPrepareTemplate(cxString code);

xmlTextReaderPtr cxXMLReaderForString(cxString code,xmlTextReaderErrorFunc error,cxAny arg);

xmlTextReaderPtr cxXMLReaderForScript(cxXMLScript this,xmlTextReaderErrorFunc error,cxAny arg);

cxEventItem cxXMLReadEvent(cxHash events, cxConstChars name, xmlTextReaderPtr reader);

#define ELEMENT_IS_TYPE(t)  (temp != NULL && strcmp(temp, #t) == 0)

#define cxXMLAppendEvent(s,o,t,e)                               \
do{                                                             \
    cxEventItem item = cxXMLReadEvent(s, #t"."#e, reader);      \
    if(item != NULL){                                           \
        CX_EVENT_APPEND(o->e, item->func,item->arg);            \
    }                                                           \
}while(0)

cxConstChars cxXMLAttrAuto(xmlTextReaderPtr reader,cxConstChars name);
#define cxXMLAttr(n)    cxXMLAttrAuto(reader,n)

cxInt cxReadFloats(cxConstChars ptr,cxFloat *values);

cxInt cxReadInts(cxConstChars ptr,cxInt *values);

cxBool cxXMLHasAttr(xmlTextReaderPtr reader,cxConstChars name);

cxConstChars cxXMLReadElementName(xmlTextReaderPtr reader);

cxString cxXMLReadString(xmlTextReaderPtr reader);

cxString cxXMLReaderReadOuterXml(xmlTextReaderPtr reader);

cxFloat cxXMLReadFloatAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxFloat value);

cxString cxXMLReadStringAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name);

cxAssist4f cxXMLReadAssist4fAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxAssist4f value);

cxColor4f cxXMLReadColor4fAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxColor4f value);

cxBox4f cxXMLReadBox4fAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxBox4f value);

cxVec2f cxXMLReadVec2fAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxVec2f value);

cxVec3f cxXMLReadVec3fAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxVec3f value);

cxFloatRange cxXMLReadFloatRangeAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxFloatRange value);

cxVec2fRange cxXMLReadVec2fRangeAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxVec2fRange value);

cxColor4fRange cxXMLReadColor4fRangeAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxColor4fRange value);

cxBox4i cxXMLReadBox4iAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxBox4i value);

cxVec2i cxXMLReadVec2iAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxVec2i value);

cxInt cxXMLReadIntAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxInt value);

cxUInt cxXMLReadUIntAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxUInt value);

cxBool cxXMLReadBoolAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxBool value);

cxTextureAttr cxXMLReadTextureAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name);

CX_C_END

#endif
