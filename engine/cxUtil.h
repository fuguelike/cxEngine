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

cxString cxUUID();

cxString cxDefaultLocalized();

cxUInt cxHexToUInt(cxConstChars bs);

cxUInt cxBinaryToUInt(cxConstChars bs);

cxBool cxCopyFile(cxConstChars file,cxCopyFileFunc func,cxAny udata);

cxAudioFileType cxAudioGetType(cxConstChars file);

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c);

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars fontName,const cxTextAttr *attr);

cxHash cxParseKeyValue(cxChars query);

cxInt cxFileFD(cxConstChars file,cxInt *off,cxInt *length);

cxInt cxDocumentFD(cxConstChars file,cxInt *off,cxInt *length);

cxInt cxAssertsFD(cxConstChars file,cxInt *off,cxInt *length);

cxString cxAssetsPath(cxConstChars file);

cxBool cxAssetsExists(cxConstChars file);

cxBool cxDocumentExists(cxConstChars file);

cxBool cxFileExists(cxConstChars file);

cxString cxLocalizedLang();

cxString cxLocalizedCountry();

cxString cxDocumentPath(cxConstChars file);

cxBool cxWriteFile(cxString file,cxString data);

CX_C_END

#endif