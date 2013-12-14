//
//  cxUtil.h
//  cxEngine
//  引擎工具方法
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxUtil_h
#define cxEngine_cxUtil_h

#include "cxConfig.h"
#include "cxString.h"
#include "cxHash.h"

CX_C_BEGIN

cxInt cxRand(cxInt min,cxInt max);

cxUInt cxHexToUInt(const cxChar *bs);

cxUInt cxBinaryToUInt(const cxChar *bs);

void cxSetRandSeed();

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c);

cxString cxCompressed(cxString data);

cxString cxDecompress(cxString data);

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars font,cxTextAttr attr);

cxString cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq);

cxHash cxParseKeyValue(cxChar *query);

cxString cxAssetsPath(cxConstChars file);

cxInt cxAssertsOpen(cxConstChars file,cxInt *start,cxInt *length);

cxDouble cxTimestamp();

cxString cxLocaleLang();

cxString cxDocumentPath(cxConstChars file);

CX_C_END

#endif