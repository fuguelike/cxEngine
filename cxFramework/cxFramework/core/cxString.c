//
//  cxString.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <ctype.h>
#include "cxString.h"

CX_OBJECT_INIT(cxString, cxObject)
{
    
}
CX_OBJECT_FREE(cxString, cxObject)
{
    utstring_done(&this->strptr);
}
CX_OBJECT_TERM(cxString, cxObject)

void cxMD5(cxString v,cxChar md5[MD5_DIGEST_LENGTH*2 + 1])
{
    static const char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    CX_ASSERT(v != NULL, "args error");
    mongo_md5_state_t state={0};
    mongo_md5_byte_t digest[MD5_DIGEST_LENGTH];
    mongo_md5_init(&state);
    mongo_md5_append(&state, (const mongo_md5_byte_t *)cxStringBody(v), cxStringLength(v));
    mongo_md5_finish(&state, digest);
    for(cxInt i = 0; i < MD5_DIGEST_LENGTH; i++){
        md5[2*i] = hex[(digest[i] & 0xf0)>> 4];
        md5[2*i + 1] = hex[digest[i] & 0x0f];
    }
}

void cxStringClean(cxString string)
{
    if(cxStringLength(string) > 0){
        utstring_clear(&string->strptr);
    }
}

cxConstChars cxStringBody(cxString string)
{
    CX_RETURN(string == NULL,NULL);
    return utstring_body(&string->strptr);
}

void cxStringConcat(cxString string,cxString str)
{
    utstring_concat(&string->strptr, &str->strptr);
}

cxInt cxStringLength(cxString string)
{
    CX_RETURN(string == NULL, 0);
    return utstring_len(&string->strptr);
}

void cxStringFormat(cxString string,cxConstChars format,...)
{
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&string->strptr, format, ap);
    va_end(ap);
}

void cxStringAppend(cxString string,cxConstChars d,cxInt l)
{
    CX_ASSERT(l > 0 && d != NULL, "args error");
    utstring_bincpy(&string->strptr, d, l);
}





