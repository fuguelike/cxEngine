//
//  cxConfig.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxConfig.h"
#include "cxXMLScript.h"

void cxObjectXMLReadAttr(cxAny pobj,cxAny newobj, xmlTextReaderPtr reader)
{
    cxObject this = newobj;
    cxObjectSetTag(this,cxXMLReadIntAttr(reader, "cxObject.tag", this->cxTag));
}

void cxObjectAutoInit(cxObject this)
{
    CX_METHOD_SET(this->XMLReadAttr, cxObjectXMLReadAttr);
}

void cxObjectAutoFree(cxObject this)
{
    
}

void cxObjectSetXMLReadFunc(cxAny obj,cxXMLReadAttrFunc func)
{
    cxObject this = obj;
    this->XMLReadAttr = func;
}

void cxObjectSetXMLReadRun(cxAny obj,cxAny pobj,cxAny newobj,xmlTextReaderPtr reader)
{
    cxObject this = obj;
    CX_RETURN(this->XMLReadAttr == NULL);
    this->XMLReadAttr(pobj,newobj,reader);
}

void cxObjectSetRoot(cxAny obj,cxAny root)
{
    cxObject this = obj;
    this->cxRoot = root;
}

cxBool cxObjectIsType(cxAny pobj,cxConstType type)
{
    cxObject this = pobj;
    CX_RETURN(this == NULL, false);
    return this->cxType == type;
}

cxAny cxObjectRoot(cxAny obj)
{
    CX_RETURN(obj == NULL, NULL);
    cxObject this = obj;
    return this->cxRoot;
}

void cxObjectSetTag(cxAny obj,cxInt tag)
{
    cxObject this = obj;
    this->cxTag = tag;
}

cxInt cxObjectGetTag(cxAny obj)
{
    cxObject this = obj;
    return this->cxTag;
}
