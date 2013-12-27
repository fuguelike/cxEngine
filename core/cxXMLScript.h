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
#include "cxEventArg.h"
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

CX_OBJECT_DEF(cxXMLScript, cxObject)
    cxString bytes;
CX_OBJECT_END(cxXMLScript)

cxString cxXMLReaderPrepareTemplate(cxString code);

xmlTextReaderPtr cxXMLReaderForString(cxString code,xmlTextReaderErrorFunc error,cxAny arg);

xmlTextReaderPtr cxXMLReaderForScript(cxXMLScript this,xmlTextReaderErrorFunc error,cxAny arg);

cxEventItem cxXMLReadEvent(cxReaderAttrInfo *info, cxConstChars name);

#define ELEMENT_IS_TYPE(t)  (temp != NULL && strcmp(temp, #t) == 0)

#define cxXMLAppendEvent(info,o,t,e)                            \
do{                                                             \
   cxEventItem item = cxXMLReadEvent(info, #t"."#e);            \
    if(item != NULL){                                           \
        CX_EVENT_APPEND(o->e, item->func,item->arg);            \
    }                                                           \
}while(0)

cxConstChars cxXMLAttr(xmlTextReaderPtr reader,cxConstChars name);

cxInt cxReadFloats(cxConstChars ptr,cxFloat *values);

cxInt cxReadInts(cxConstChars ptr,cxInt *values);

cxBool cxXMLHasAttr(xmlTextReaderPtr reader,cxConstChars name);

cxConstChars cxXMLReadElementName(xmlTextReaderPtr reader);

cxString cxXMLReadString(xmlTextReaderPtr reader);

cxString cxXMLReaderReadOuterXml(xmlTextReaderPtr reader);

cxAny cxXMLReadTypesAttr(cxReaderAttrInfo *info, cxConstChars name);

cxFloat cxXMLReadFloatAttr(cxReaderAttrInfo *info, cxConstChars name,cxFloat value);

cxString cxXMLReadStringAttr(cxReaderAttrInfo *info,cxConstChars name);

cxAssist4f cxXMLReadAssist4fAttr(cxReaderAttrInfo *info, cxConstChars name,cxAssist4f value);

cxColor4f cxXMLReadColor4fAttr(cxReaderAttrInfo *info, cxConstChars name,cxColor4f value);

cxBox4f cxXMLReadBox4fAttr(cxReaderAttrInfo *info, cxConstChars name,cxBox4f value);

cxVec2f cxXMLReadVec2fAttr(cxReaderAttrInfo *info,cxConstChars name,cxVec2f value);

cxSize2f cxXMLReadSize2fAttr(cxReaderAttrInfo *info,cxConstChars name,cxSize2f value);

cxVec3f cxXMLReadVec3fAttr(cxReaderAttrInfo *info,cxConstChars name,cxVec3f value);

cxFloatRange cxXMLReadFloatRangeAttr(cxReaderAttrInfo *info,cxConstChars name,cxFloatRange value);

cxVec2fRange cxXMLReadVec2fRangeAttr(cxReaderAttrInfo *info,cxConstChars name,cxVec2fRange value);

cxColor4fRange cxXMLReadColor4fRangeAttr(cxReaderAttrInfo *info,cxConstChars name,cxColor4fRange value);

cxBox4i cxXMLReadBox4iAttr(cxReaderAttrInfo *info, cxConstChars name,cxBox4i value);

cxVec2i cxXMLReadVec2iAttr(cxReaderAttrInfo *info, cxConstChars name,cxVec2i value);

cxInt cxXMLReadIntAttr(cxReaderAttrInfo *info,cxConstChars name,cxInt value);

cxUInt cxXMLReadUIntAttr(cxReaderAttrInfo *info, cxConstChars name,cxUInt value);

cxBool cxXMLReadBoolAttr(cxReaderAttrInfo *info,cxConstChars name,cxBool value);

cxTextureAttr cxXMLReadTextureAttr(cxReaderAttrInfo *info,cxConstChars name);

CX_C_END

#endif
