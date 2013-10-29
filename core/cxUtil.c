//
//  cxUtil.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <evhttp.h>
#include <sys/time.h>
#include "cxBase.h"
#include "cxUtil.h"

void cxSetRandSeed()
{
    srand(clock());
}

cxBool cxParseKeyValue(cxChar *query,cxChar *key,cxChar *value)
{
    cxBool ret = false;
    struct evkeyvalq kv={0};
    if(evhttp_parse_query_str(query, &kv) != 0){
        return ret;
    }
	struct evkeyval *header;
	for (header = kv.tqh_first; header != NULL; header = header->next.tqe_next) {
        memcpy(key, header->key, strlen(header->key));
        memcpy(value, header->value, strlen(header->value));
        ret = true;
	}
    evhttp_clear_headers(&kv);
    return ret;
}

cxInt cxParseQuery(cxConstChars query,cxChar *key,cxChar *value)
{
    CX_ASSERT(query != NULL, "query args error");
    cxInt rv = 0;
    cxInt len = strlen(query);
    cxChar *sq = strrchr(query, '=');
    if(sq != NULL){
        cxInt cl = len - (cxInt)strlen(sq);
        memcpy(key, query, cl);
        key[cl]='\0';
        memcpy(value, sq + 1, strlen(sq));
        rv = 2;
    }else{
        memcpy(key, query, len + 1);
        rv = 1;
    }
    return rv;
}

cxInt cxParseURL(cxConstChars url,cxChar *path,cxChar *query)
{
    CX_ASSERT(url != NULL && path != NULL && query != NULL, "args error");
    cxInt rv = 0;
    cxInt len = strlen(url);
    cxChar *sq = strrchr(url, '?');
    if(sq != NULL){
        cxInt cl = len - (cxInt)strlen(sq);
        memcpy(path, url, cl);
        path[cl]='\0';
        memcpy(query, sq + 1, strlen(sq));
        rv = 2;
    }else{
        memcpy(path, url, len + 1);
        rv = 1;
    }
    return rv;
}

cxULong cxNextPot(cxULong x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return x + 1;
}

cxDouble cxTimestamp()
{
    struct timeval val;
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}

void cxUtilInfo(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("INFO", file, line, format, ap);
    va_end(ap);
}

void cxUtilError(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("ERROR", file, line, format, ap);
    va_end(ap);
}

void cxUtilWarn(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("WARN", file, line, format, ap);
    va_end(ap);
}

void cxUtilAssert(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("Assert", file, line, format, ap);
    va_end(ap);
}