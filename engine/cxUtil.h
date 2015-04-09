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
}cxProgressInfo;

void cxSetRecvjsonFunc(cxAny func);

typedef void (*cxCopyFileFunc)(cxConstChars file,cxProgressInfo *progress,cxAny udata);

cxStr cxUUID();

cxStr cxDefaultLocalized(cxConstChars country,cxConstChars lang);

cxUInt cxHexToUInt(cxConstChars bs);

cxUInt cxBinaryToUInt(cxConstChars bs);

cxBool cxCopyFile(cxConstChars file,cxCopyFileFunc func,cxAny udata);

cxAudioFileType cxAudioGetType(cxConstChars file);

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c);

cxStr cxCreateTXTTextureData(cxConstChars txt,cxConstChars fontName,const cxTextAttr *attr);

cxHash cxParseKeyValue(cxChars query);

cxInt cxFileFD(cxConstChars file,cxInt *off,cxInt *length);

cxInt cxDocumentFD(cxConstChars file,cxInt *off,cxInt *length);

cxInt cxAssertsFD(cxConstChars file,cxInt *off,cxInt *length);

cxStr cxAssetsPath(cxConstChars file);

cxBool cxAssetsExists(cxConstChars file);

cxBool cxDocumentExists(cxConstChars file);

cxBool cxFileExists(cxConstChars file);

cxStr cxLocalizedLang();

cxStr cxLocalizedCountry();

cxStr cxDocumentPath(cxConstChars file);

cxBool cxWriteFile(cxStr file,cxStr data);

CX_C_END

#endif