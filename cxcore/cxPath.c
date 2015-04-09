//
//  cxPath.c
//  cxEngine
//
//  Created by xuhua on 12/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxStr.h"
#include "cxPath.h"

CX_TYPE(cxPath, cxObject)
{}
CX_INIT(cxPath, cxObject)
{}
CX_FREE(cxPath, cxObject)
{}
CX_TERM(cxPath, cxObject)

//parse url?key=value
cxPath cxPathParseKeyValue(cxPath this)
{
    CX_RETURN(this->count == 1, this);
    cxConstChars url = this->key;
    cxInt len = (cxInt)strlen(url);
    cxChars sq = strrchr(url, '=');
    if(sq == NULL){
        return this;
    }
    cxInt kl = len - (cxInt)strlen(sq);
    this->key[kl]='\0';
    cxInt vl = (cxInt)strlen(sq + 1);
    memcpy(this->value, sq + 1, vl);
    this->value[vl]='\0';
    this->count = 3;
    return this;
}

//parse url?key
cxPath cxPathParse(cxConstChars url)
{
    cxPath this = CX_CREATE(cxPath);
    if(!cxConstCharsOK(url)){
        return this;
    }
    CX_ASSERT(strlen(url) < CX_HASH_MAX_KEY, "url too long");
    this->count = 0;
    cxInt len = (cxInt)strlen(url);
    cxChars sq = strrchr(url, '?');
    if(sq != NULL){
        cxInt cl = len - (cxInt)strlen(sq);
        memcpy(this->path, url, cl);
        this->path[cl]='\0';
        cxInt kl = (cxInt)strlen(sq);
        memcpy(this->key, sq + 1, kl);
        this->key[kl]='\0';
        this->count = 2;
    }else{
        memcpy(this->path, url, len);
        this->path[len]='\0';
        this->count = 1;
    }
    return cxPathParseKeyValue(this);
}

