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

// -1 <-> 1
#define CX_RAND_11f() ((2.0f*((cxFloat)rand()/(cxFloat)RAND_MAX))-1.0f)

// 0 <-> 1
#define CX_RAND_01f() ((cxFloat)rand()/(cxFloat)RAND_MAX)

cxInt cxRand(cxInt min,cxInt max);

void cxSetRandSeed();

cxString cxCompressed(cxString data);

cxString cxDecompress(cxString data);

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars font,cxTextAttr attr);

cxString cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq);

cxString cxMP3SamplesWithData(cxString data,cxUInt *format,cxUInt *freq);

cxString cxMP3SamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq);

cxHash cxParseKeyValue(cxChar *query);

cxInt cxParseURL(cxConstChars url,cxChar *path,cxChar *query);

cxInt cxParseQuery(cxConstChars query,cxChar *key,cxChar *value);

cxString cxAssetsPath(cxConstChars file);

cxDouble cxTimestamp();

cxString cxLocaleLang();

cxString cxDocumentPath(cxConstChars file);

CX_C_END

#endif