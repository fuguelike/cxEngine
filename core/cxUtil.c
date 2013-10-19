//
//  cxUtil.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <sys/time.h>
#include "cxBase.h"
#include "cxUtil.h"

void cxSetRandSeed()
{
    srand(clock());
}

cxInt cxParseURL(cxString url,cxChar *path,cxChar *query)
{
    CX_ASSERT(url != NULL && path != NULL && query != NULL, "args error");
    cxInt rv = 0;
    cxConstChars ptr = cxStringBody(url);
    cxInt len = cxStringLength(url);
    cxChar *sq = strrchr(ptr, '?');
    if(sq != NULL){
        cxInt cl = len - (cxInt)strlen(sq);
        memcpy(path, ptr, cl);
        path[cl]='\0';
        memcpy(query, sq + 1, strlen(sq));
        rv = 2;
    }else{
        memcpy(path, ptr, len + 1);
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