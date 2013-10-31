//
//  cxString.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxString.h"

CX_OBJECT_INIT(cxString, cxObject)
{
    
}
CX_OBJECT_FREE(cxString, cxObject)
{
    utstring_done(&this->strptr);
}
CX_OBJECT_TERM(cxString, cxObject)

void cxStringClean(cxString string)
{
    if(cxStringLength(string) > 0){
        utstring_clear(&string->strptr);
    }
}

cxBool cxStringEqu(cxString s1,cxString s2)
{
    if(s1 == NULL || s2 == NULL){
        return false;
    }
    if(s1->strptr.i != s2->strptr.i){
        return false;
    }
    return memcmp(s1->strptr.d, s2->strptr.d,s1->strptr.i) == 0;
}

cxString cxStringAllocChars(cxConstChars str)
{
    CX_ASSERT(str != NULL, "str null");
    cxString rv = CX_ALLOC(cxString);
    cxStringAppend(rv, (void *)str, (cxInt)strlen(str));
    return rv;
}

cxString cxStringConstChars(cxConstChars str)
{
    CX_ASSERT(str != NULL, "str null");
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, (void *)str, (cxInt)strlen(str));
    return rv;
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

cxString cxStringBinary(cxChar *d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, d, l);
    return rv;
}

cxString cxStringAttach(cxChar *d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    return rv;
}

cxString cxStringCreate(cxConstChars format,...)
{
    cxString rv = CX_CREATE(cxString);
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&rv->strptr, format, ap);
    va_end(ap);
    return rv;
}

void cxStringFormat(cxString string,cxConstChars format,...)
{
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&string->strptr, format, ap);
    va_end(ap);
}

void cxStringAppend(cxString string,cxChar *d,cxInt l)
{
    utstring_bincpy(&string->strptr, d, l);
}