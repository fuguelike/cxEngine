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

cxChar *cxXMLAttrAuto(xmlTextReaderPtr reader,cxConstChars name)
{
    xmlChar *txt = xmlTextReaderGetAttribute(reader, BAD_CAST(name));
    if(txt != NULL && xmlStrlen(txt) > 0){
        return (cxChar *)txt;
    }
    xmlFree(txt);
    return NULL;
}


//method({json}
cxEventItem cxXMLReadEvent(cxHash events,cxConstChars name,xmlTextReaderPtr reader)
{
    //xml view event event(arg)
    cxChar *eventName = cxXMLAttr(name);
    CX_RETURN(eventName == NULL,NULL);
    cxConstChars ptr = eventName;
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
    cxChar *svalue = cxXMLAttr(name);
    if(svalue != NULL){
        value = atof(svalue);
        xmlFree(svalue);
    }
    return value;
}

cxInt cxXMLReadFloatsAttr(xmlTextReaderPtr reader,cxConstChars name,cxFloat *values)
{
    cxChar *svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL,0);
    cxConstChars ptr = svalue;
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

cxChar *cxXMLReadString(xmlTextReaderPtr reader)
{
    return (cxChar *)xmlTextReaderReadString(reader);
}

cxInt cxXMLReadIntAttr(xmlTextReaderPtr reader,cxConstChars name,cxInt value)
{
    cxChar *svalue = cxXMLAttr(name);
    if(svalue != NULL){
        value = atoi(svalue);
    }
    xmlFree(svalue);
    return value;
}

cxBool cxXMLReadBoolAttr(xmlTextReaderPtr reader,cxConstChars name,cxBool value)
{
    cxBool ret = value;
    cxChar *svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL,ret);
    if(strcasecmp(svalue, "true") == 0){
        ret = true;
    }else if(strcasecmp(svalue, "false") == 0){
        ret = false;
    }else{
        CX_ERROR("bool value is true or false");
    }
    xmlFree(svalue);
    return ret;
}








