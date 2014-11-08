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

#if (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
CX_DEF(cxBuffer, cxObject)
    ALuint buffer;
    ALenum format;
    ALsizei freq;
CX_END(cxBuffer, cxObject)

CX_DEF(cxTrack, cxObject)
    ALsizei freq;
    ALuint format;
    ALuint source;
CX_END(cxTrack, cxObject)

CX_DEF(cxPlayer, cxObject)
    cxArray tracks;
    cxHash caches;
    ALCdevice *device;
    ALCcontext *context;
CX_END(cxPlayer, cxObject)
#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID

CX_DEF(cxTrack, cxObject)
    cxInt soundId;
    cxString file;
CX_END(cxTrack, cxObject)

CX_DEF(cxBuffer, cxObject)

CX_END(cxBuffer, cxObject)

CX_DEF(cxPlayer, cxObject)
    cxHash tracks;
CX_END(cxPlayer, cxObject)

#endif

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
