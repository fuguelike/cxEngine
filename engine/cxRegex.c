//
//  cxRegex.c
//  cxEngine
//
//  Created by xuhua on 12/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxRegex.h"

CX_OBJECT_TYPE(cxRegex, cxObject)
{
    
}
CX_OBJECT_INIT(cxRegex, cxObject)
{
    
}
CX_OBJECT_FREE(cxRegex, cxObject)
{
    if(this->regex != NULL){
        pcre_free(this->regex);
    }
    CX_RELEASE(this->input);
}
CX_OBJECT_TERM(cxRegex, cxObject)

cxString cxRegexReplace(cxRegex this,cxRegexReplaceFunc replaceFunc,cxAny arg)
{
    CX_ASSERT(replaceFunc != NULL, "args error");
    cxString ret = CX_CREATE(cxString);
    cxInt start = 0;
    cxInt length = cxStringLength(this->input);
    cxConstChars ptr = cxStringBody(this->input);
    while(cxRegexNext(this)){
        cxRegexPos pos = cxRegexPosition(this, 0);
        if((pos.start - start) > 0){
            cxStringAppend(ret, ptr + start, pos.start - start);
        }
        cxString replace = replaceFunc(this,arg);
        if(cxStringOK(replace)){
            cxStringConcat(ret, replace);
        }
        start = pos.end;
    }
    if(start < length){
        cxStringAppend(ret, ptr + start, length - start);
    }
    return cxStringLength(ret) > 0 ? ret : NULL;
}

cxRegex cxRegexCreate(cxConstChars regex, cxString input,cxUInt flags)
{
    cxRegex this = CX_CREATE(cxRegex);
    if(flags == 0){
        flags = PCRE_DOTALL | PCRE_CASELESS;
    }
    this->regex = pcre_compile(regex, flags, &this->error_message, &this->error_offset, NULL);
    CX_ASSERT(this->regex != NULL, "pcre compile error");
    CX_RETAIN_SWAP(this->input, input);
    return this;
}

cxInt cxRegexLength(cxRegex this)
{
    return this->count;
}

cxArray cxRegexMatchAll(cxRegex this)
{
    cxArray rv = CX_CREATE(cxArray);
    for(cxInt i=0; i < this->count;i++){
        cxString item = cxRegexMatch(this, i);
        CX_CONTINUE(item == NULL);
        cxArrayAppend(rv, item);
    }
    return rv;
}

cxString cxRegexMatch(cxRegex this,cxInt index)
{
    if(index < 0 || index >= this->count){
        return NULL;
    }
    cxRegexPos pos = cxRegexPosition(this, index);
    if(!CX_REGEX_POS_OK(pos)){
        return NULL;
    }
    cxString ret = CX_CREATE(cxString);
    if((pos.end - pos.start) > 0){
        cxStringAppend(ret, (cxChar *)(cxStringBody(this->input) + pos.start), pos.end - pos.start);
    }
    return ret;
}

cxRegexPos cxRegexPosition(cxRegex this,cxInt index)
{
    if(index < 0 || index >= this->count){
        return (cxRegexPos){-1 ,-1};
    }
    return (cxRegexPos){this->offsets[index*2],this->offsets[index*2 + 1]};
}

cxBool cxRegexNext(cxRegex this)
{
    CX_ASSERT(this->input != NULL, "input string error");
    cxConstChars body = cxStringBody(this->input);
    cxInt length = cxStringLength(this->input);
    this->count = pcre_exec(this->regex, 0, body, length, this->start, 0, this->offsets, CX_REGEX_MAX_COUNT);
    if(this->count > 0){
        cxRegexPos p = cxRegexPosition(this, 0);
        this->start = p.end;
        return true;
    }
    return false;
}


