//
//  cxUrlPath.c
//  cxEngine
//
//  Created by xuhua on 12/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxUrlPath.h"

CX_OBJECT_TYPE(cxUrlPath, cxObject)
{}
CX_OBJECT_INIT(cxUrlPath, cxObject)
{}
CX_OBJECT_FREE(cxUrlPath, cxObject)
{}
CX_OBJECT_TERM(cxUrlPath, cxObject)

static cxUrlPath cxUrlPathParseKeyValueImp(cxUrlPath this)
{
    CX_RETURN(this->count == 1, this);
    cxConstChars url = this->key;
    cxInt len = strlen(url);
    cxChars sq = strrchr(url, '=');
    if(sq == NULL){
        return this;
    }
    cxInt kl = len - (cxInt)strlen(sq);
    this->key[kl]='\0';
    cxInt vl = strlen(sq + 1);
    memcpy(this->value, sq + 1, vl);
    this->value[vl]='\0';
    this->count = 3;
    return this;
}

cxUrlPath cxUrlPathParse(cxConstChars url)
{
    cxUrlPath this = CX_CREATE(cxUrlPath);
    if(!cxConstCharsOK(url)){
        return this;
    }
    CX_ASSERT(strlen(url) < CX_MAX_KEY, "url too long");
    this->count = 0;
    cxInt len = strlen(url);
    cxChars sq = strrchr(url, '?');
    if(sq != NULL){
        cxInt cl = len - (cxInt)strlen(sq);
        memcpy(this->path, url, cl);
        this->path[cl]='\0';
        cxInt kl = strlen(sq);
        memcpy(this->key, sq + 1, kl);
        this->key[kl]='\0';
        this->count = 2;
    }else{
        memcpy(this->path, url, len);
        this->path[len]='\0';
        this->count = 1;
    }
    return cxUrlPathParseKeyValueImp(this);
}

