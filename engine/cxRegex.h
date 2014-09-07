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
#include <cxcore/cxBase.h>

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
CX_OBJECT_END(cxRegex, cxObject)

typedef cxString (*cxRegexReplaceFunc)(cxRegex regex,cxAny arg);

cxString cxRegexReplace(cxAny preg,cxRegexReplaceFunc replaceFunc,cxAny arg);

cxRegex cxRegexCreate(cxConstChars regex, cxString input,cxUInt flags);

cxInt cxRegexLength(cxAny preg);

cxArray cxRegexMatchAll(cxAny preg);

cxString cxRegexMatch(cxAny preg,cxInt index);

cxRegexPos cxRegexPosition(cxAny preg,cxInt index);

cxBool cxRegexNext(cxAny preg);

CX_C_END

#endif
