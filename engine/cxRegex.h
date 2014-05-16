//
//  cxRegex.h
//  cxEngine
//
//  Created by xuhua on 12/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxRegex_h
#define cxEngine_cxRegex_h

#include <pcre/pcre.h>
#include "cxString.h"

CX_C_BEGIN

#define CX_REGEX_MAX_COUNT 16

typedef struct{
    cxInt start;
    cxInt end;
}cxRegexPos;

#define CX_REGEX_POS_OK(p) ((p).start >= 0 && (p).end >= 0)

CX_OBJECT_DEF(cxRegex, cxObject)
    int error_offset;
    const char *error_message;
    pcre *regex;
    int offsets[CX_REGEX_MAX_COUNT];
    cxString input;
    int start;
    int count;
CX_OBJECT_END(cxRegex)

typedef cxString (*cxRegexReplaceFunc)(cxRegex regex,cxAny arg);

cxString cxRegexReplace(cxRegex this,cxRegexReplaceFunc replaceFunc,cxAny arg);

cxRegex cxRegexCreate(cxConstChars regex, cxString input,cxUInt flags);

cxInt cxRegexLength(cxRegex this);

cxArray cxRegexMatchAll(cxRegex this);

cxString cxRegexMatch(cxRegex this,cxInt index);

cxRegexPos cxRegexPosition(cxRegex this,cxInt index);

cxBool cxRegexNext(cxRegex this);

CX_C_END

#endif
