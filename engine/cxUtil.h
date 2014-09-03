//
//  cxUtil.h
//  cxEngine
//  引擎工具方法
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxUtil_h
#define cxEngine_cxUtil_h

#include <cxcore/cxBase.h>
#include "cxPlayer.h"
#include "cxMath.h"

CX_C_BEGIN

typedef struct {
    cxInt total;
    cxInt current;
}cxProgress;

typedef void (*cxCopyFileFunc)(cxConstChars file,cxProgress *progress,cxAny udata);

cxArray cxFontNames();

cxInt cxRand(cxInt min,cxInt max);

cxUInt cxHexToUInt(cxConstChars bs);

cxUInt cxBinaryToUInt(cxConstChars bs);

void cxSetRandSeed();

cxBool cxCopyFile(cxConstChars file,cxCopyFileFunc func,cxAny udata);

cxAudioFileType cxAudioGetType(cxConstChars file);

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c);

cxString cxAESDecode(cxString data,cxString key);

cxString cxAESEncode(cxString data,cxString key);

cxString cxCompressed(cxString data);

cxString cxDecompress(cxString data);

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars fontName,cxTextAlign align,cxFloat size, cxTextureTXTConfig *info);

cxHash cxParseKeyValue(cxChars query);

cxString cxAssetsPath(cxConstChars file);

cxBool cxAssetsExists(cxConstChars file);

cxBool cxDocumentExists(cxConstChars file);

cxDouble cxTimestamp();

cxString cxLocalizedLang();

cxString cxDocumentPath(cxConstChars file);

cxBool cxWriteFile(cxString file,cxString data);

CX_C_END

#endif