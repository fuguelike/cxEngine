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

cxString cxCreateTXTTextureData(cxConstChars txt,
                                cxConstChars fontName,cxFloat size,
                                cxTextAlign align,
                                cxInt cw,cxInt ch,
                                cxColor4f color,
                                cxColor4f shadowColor,cxFloat shadowBlur,cxSize2f shadowOffset,
                                cxColor4f strokeColor,cxFloat strokeWidth
                                );

cxHash cxParseKeyValue(cxChars query);

cxInt cxFileFD(cxConstChars file,cxInt *off,cxInt *length);

cxInt cxDocumentFD(cxConstChars file,cxInt *off,cxInt *length);

cxInt cxAssertsFD(cxConstChars file,cxInt *off,cxInt *length);

cxString cxAssetsPath(cxConstChars file);

cxBool cxAssetsExists(cxConstChars file);

cxBool cxDocumentExists(cxConstChars file);

cxBool cxFileExists(cxConstChars file);

cxDouble cxTimestamp();

cxString cxLocalizedLang();

cxString cxDocumentPath(cxConstChars file);

cxBool cxWriteFile(cxString file,cxString data);

CX_C_END

#endif