//
//  cxXMLScript.c
//  cxEngine
//
//  Created by xuhua on 10/14/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxXMLScript.h"
#include "cxHash.h"
#include "cxEventItem.h"
#include "cxEngine.h"

CX_OBJECT_INIT(cxXMLScript, cxObject)
{
    
}
CX_OBJECT_FREE(cxXMLScript, cxObject)
{
    CX_RELEASE(this->bytes);
}
CX_OBJECT_TERM(cxXMLScript, cxObject)

xmlChar *cxXMLAttrAuto(xmlTextReaderPtr reader,cxConstChars name)
{
    xmlChar *txt = xmlTextReaderGetAttribute(reader, BAD_CAST(name));
    if(txt != NULL && xmlStrlen(txt) > 0){
        return txt;
    }
    xmlFree(txt);
    return NULL;
}


//method({json}
cxEventItem cxXMLReadEvent(cxHash events,cxConstChars name,xmlTextReaderPtr reader)
{
    //xml view event event(arg)
    xmlChar *eventName = cxXMLAttr(name);
    CX_RETURN(eventName == NULL,NULL);
    cxConstChars ptr = (cxConstChars)eventName;
    cxInt len = (cxInt)strlen(ptr) + 1;
    cxInt lpos = 0;
    cxInt rpos = 0;
    for(int i=0; i < len; i++){
        if(ptr[i] == '('){
            lpos = i;
        }
        if(ptr[i] == ')'){
            rpos = i;
        }
    }
    //parse simple json object arg
    cxChar eName[len];
    cxChar eArg[len];
    if(lpos > 0 && rpos > 0){
        int len = rpos - lpos - 1;
        memcpy(eArg, ptr + lpos + 1, len);
        eArg[len] = '\0';
        //char ' -> "
        for(int i=0; i < len;i++){
            if(eArg[i] == '\''){
                eArg[i] = '\"';
            }
        }
        //copy name
        len = lpos;
        memcpy(eName, ptr, len);
        eName[len] = '\0';
    }else{
        memcpy(eName, ptr, len);
    }
    cxEventItem event = cxHashGet(events, cxHashStrKey(eName));
    //global engine event
    if(event == NULL){
        event = cxEngineGetEvent(eName);
    }
    //set event arg
    if(event != NULL && lpos > 0 && rpos > 0){
        cxAny arg = cxEventArgCreate(eArg);
        CX_RETAIN_SWAP(event->arg, arg);
    }
    xmlFree(eventName);
    return event;
}

cxVec2f cxXMLReadVec2fAttr(xmlTextReaderPtr reader,cxConstChars name,cxVec2f value)
{
    cxXMLReadFloatsAttr(reader, name, &value.x);
    return value;
}

cxFloat cxXMLReadFloatAttr(xmlTextReaderPtr reader,cxConstChars name,cxFloat value)
{
    xmlChar *svalue = cxXMLAttr(name);
    if(svalue != NULL){
        value = atof((cxConstChars)svalue);
        xmlFree(svalue);
    }
    return value;
}

cxInt cxXMLReadFloatsAttr(xmlTextReaderPtr reader,cxConstChars name,cxFloat *values)
{
    xmlChar *svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL,0);
    cxConstChars ptr = (cxConstChars)svalue;
    cxInt len = (cxInt)strlen(ptr) + 1;
    cxInt argLen = 0;
    cxChar arg[16]={0};
    cxInt c = 0;
    for(int i=0; i < len; i++){
        if(ptr[i] == ',' || ptr[i] == '\0'){
            arg[argLen] = '\0';
            cxFloat v = atof(arg);
            c++;
            (*values++) = v;
            argLen = 0;
        }else{
            arg[argLen++] = ptr[i];
        }
    }
    xmlFree(svalue);
    return c;
}

//r,g,b,a
cxColor4f cxXMLReadColorAttr(xmlTextReaderPtr reader,cxConstChars name,cxColor4f color)
{
    cxXMLReadFloatsAttr(reader, name, &color.r);
    return color;
}

cxInt cxXMLReadIntAttr(xmlTextReaderPtr reader,cxConstChars name,cxInt value)
{
    xmlChar *svalue = cxXMLAttr(name);
    if(svalue != NULL){
        value = atoi((cxConstChars)svalue);
    }
    xmlFree(svalue);
    return value;
}

cxBool cxXMLReadBoolAttr(xmlTextReaderPtr reader,cxConstChars name,cxBool value)
{
    xmlChar *svalue = cxXMLAttr(name);
    if(svalue != NULL){
        value = (xmlStrcasecmp(svalue, BAD_CAST"true") == 0);
    }
    xmlFree(svalue);
    return value;
}