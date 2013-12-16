//
//  cxPlayer.h
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxPlayer_h
#define cxEngine_cxPlayer_h

#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)

#elif (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #error "not define target platform"
#endif

#include "cxBase.h"
#include "cxArray.h"
#include "cxString.h"

CX_C_BEGIN

typedef enum {
    cxAudioFileTypeNone,
    cxAudioFileTypeMP3,
    cxAudioFileTypeWAV,
}cxAudioFileType;

void cxPauseEffect(cxAny this);

void cxResumeEffect(cxAny this);

void cxStopEffect(cxAny this);

cxAny cxPlayEffect(cxConstChars file,cxBool loop);

void cxPlayMusic(cxConstChars file,cxBool loop);

void cxStopMusic();

void cxPauseMusic();

void cxResumeMusic();

#define INIT_TRACK 5

void cxPlayerOpen(cxInt freq,cxInt format);

void cxPlayerDestroy();

cxAny cxPlayerInstance();

CX_C_END

#endif
