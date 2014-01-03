//
//  cxViewRoot.h
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxViewRoot_h
#define cxEngine_cxViewRoot_h

#include <core/cxView.h>
#include <core/cxEventArg.h>
#include <streams/cxAssetsStream.h>
#include <textures/cxTextureFactory.h>
#include <views/cxSprite.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxViewRoot, cxView)
    cxBool isError;
    cxHash items;
    cxHash actions;
    CX_METHOD_ALLOC(cxAny, Make, cxConstChars, xmlTextReaderPtr);
    CX_EVENT_ALLOC(onBegin);
    CX_EVENT_ALLOC(onEnd);
CX_OBJECT_END(cxViewRoot)

void cxViewRootSet(cxReaderAttrInfo *info);

void cxViewRootSetItem(cxAny pview,cxConstChars key,cxAny item);

cxAny cxViewRootMakeElement(cxConstChars temp,xmlTextReaderPtr reader);

void cxViewRootReadAttr(cxReaderAttrInfo *info);

cxViewRoot cxViewRootCreate(cxConstChars xml);

void cxViewRootRemove(cxAny pview,cxConstChars key);

cxAny cxViewRootGet(cxAny pview,cxConstChars key);

cxAny cxViewRootMake(cxString xml);

cxBool cxViewRootLoad(cxAny pview,cxConstChars xml);

cxBool cxViewRootLoadWithReader(cxAny pview,xmlTextReaderPtr reader);

CX_C_END

#endif







