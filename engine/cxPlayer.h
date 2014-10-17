//
//  cxPlayer.h
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxPlayer_h
#define cxEngine_cxPlayer_h

#include <cxcore/cxBase.h>

#if (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

CX_C_BEGIN

#define INIT_TRACK 5

typedef enum {
    cxAudioFileTypeNone,
    cxAudioFileTypeMP3,
    cxAudioFileTypeWAV,
}cxAudioFileType;

CX_DEF(cxPlayer, cxObject)
#if (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
    cxArray tracks;
    cxHash caches;
    ALCdevice *device;
    ALCcontext *context;
    CX_SLOT_ALLOC(onMemory);
#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID
    cxHash tracks;
#endif
CX_END(cxPlayer, cxObject)

void cxPauseEffect(cxAny this);

void cxResumeEffect(cxAny this);

void cxStopEffect(cxAny this);

cxAny cxPlayEffect(cxConstChars file,cxBool loop);

void cxPlayMusic(cxConstChars file,cxBool loop);

void cxStopMusic();

void cxPauseMusic();

void cxResumeMusic();

CX_C_END

#endif
