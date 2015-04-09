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
#include "cxStr.h"

CX_C_BEGIN

#define CX_REGEX_MAX_COUNT 16

typedef struct{
    cxInt start;
    cxInt end;
}cxRegexPos;

#define CX_REGEX_POS_OK(p) ((p).start >= 0 && (p).end >= 0)

CX_DEF(cxRegex, cxObject)
    cxInt error_offset;
    cxConstChars error_message;
    pcre *regex;
    cxInt offsets[CX_REGEX_MAX_COUNT];
    cxStr input;
    cxInt start;
    cxInt count;
CX_END(cxRegex, cxObject)

typedef cxStr (*cxRegexReplaceFunc)(cxRegex regex,cxInt idx,cxAny arg);

cxStr cxRegexReplace(cxAny preg,cxRegexReplaceFunc replaceFunc,cxAny arg);

cxRegex cxRegexCreate(cxConstChars regex, cxStr input,cxUInt flags);

cxInt cxRegexLength(cxAny preg);

cxArray cxRegexMatchAll(cxAny preg);

cxStr cxRegexMatch(cxAny preg,cxInt index);

cxRegexPos cxRegexPosition(cxAny preg,cxInt index);

cxBool cxRegexNext(cxAny preg);

CX_C_END

#endif
