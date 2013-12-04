//
//  cxString.h
//  cxEngine
//  可以存放二进制数据
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxString_h
#define cxEngine_cxString_h

#include <utstring.h>
#include "cxBase.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxString, cxObject)
    UT_string strptr;
CX_OBJECT_END(cxString)

//create static cxString

#define cxStringStatic(_s_)             \
&(struct cxString){                     \
    .super.cxType=cxStringAutoType,     \
    .super.cxRefcount=1,                \
    .strptr.d=(char *)(_s_),            \
    .strptr.n=0,                        \
    .strptr.i=strlen(_s_)               \
}

#define cxStringData(_d_,_l_)           \
&(struct cxString){                     \
    .super.cxType=cxStringAutoType,     \
    .super.cxRefcount=1,                \
    .strptr.d=(char *)(_d_),            \
    .strptr.n=0,                        \
    .strptr.i=(_l_)                     \
}

#define cxStringValue(v,t)     cxStringData(&(t){v},sizeof(t))

#define cxConstCharsEqu(s1,s2) ((s1) != NULL && (s2) != NULL && strcmp(s1,s2) == 0)

cxString cxStringAttach(cxChar *d,cxInt l);

void cxStringClean(cxString string);

cxString cxMD5(cxString v);

cxBool cxStringEqu(cxString s1,cxString s2);

cxInt cxStringLength(cxString string);

cxConstChars cxStringBody(cxString string);

cxString cxStringAllocChars(cxConstChars str);

cxString cxStringConstChars(cxConstChars str);

void cxStringConcat(cxString string,cxString str);

void cxStringFormat(cxString string,cxConstChars format,...);

cxString cxStringCreate(cxConstChars format,...);

#define UTF8(f,...)  cxStringCreate(f,##__VA_ARGS__)

void cxStringAppend(cxString string,cxChar *d,cxInt l);

cxString cxStringBinary(cxChar *d,cxInt l);

CX_SCRIPT_DEF(cxString);

CX_C_END

#endif
