//
//  cxHashXML.h
//  cxEngine
//
//  Created by xuhua on 10/19/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxHashXML_h
#define cxEngine_cxHashXML_h

#include "cxBase.h"
#include "cxHash.h"
#include "cxXMLScript.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxHashXML, cxObject)
    cxHash items;
    cxBool isError;
    CX_METHOD_DEF(cxXMLScriptMakeElementFunc, Make);
CX_OBJECT_END(cxHashXML)

cxTypes cxHashXMLReadAtlasBoxPoint(cxHashXML xml,cxConstChars texfile,xmlTextReaderPtr reader);

cxTypes cxHashXMLReadString(cxHashXML xml,xmlTextReaderPtr reader);

void cxHashXMLReadDBEnv(cxHashXML xml,xmlTextReaderPtr reader);

cxBool cxHashXMLLoad(cxAny hashxml,cxConstChars xml);

cxHashXML cxHashXMLCreate(cxConstChars xml);

cxTypes cxHashXMLReadNumber(cxHashXML xml,cxConstChars temp,xmlTextReaderPtr reader);

cxBool cxHashXMLLoadWithReader(cxAny hash,xmlTextReaderPtr reader);

cxTypes cxHashXMLReadHash(cxHashXML xml,xmlTextReaderPtr reader);

cxTypes cxHashXMLReadArray(cxHashXML xml,xmlTextReaderPtr reader);

CX_C_END

#endif
