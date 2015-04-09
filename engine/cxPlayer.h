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

CX_C_BEGIN

#define INIT_TRACK 8

typedef enum {
    cxAudioFileTypeNone,
    cxAudioFileTypeMP3,
    cxAudioFileTypeWAV,
    cxAudioFileTypePCM,
}cxAudioFileType;

#if (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
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
    cxInt channels;
    cxInt samples;
    cxArray tracks;
    cxHash caches;
    ALCdevice *device;
    ALCcontext *context;
CX_END(cxPlayer, cxObject)
#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
CX_DEF(cxTrack, cxObject)
    SLObjectItf bqPlayerObject;
    SLPlayItf bqPlayerPlay;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
    SLEffectSendItf bqPlayerEffectSend;
    SLMuteSoloItf bqPlayerMuteSolo;
    SLVolumeItf bqPlayerVolume;
CX_END(cxTrack, cxObject)

CX_DEF(cxBuffer, cxObject)
    cxStr data;
CX_END(cxBuffer, cxObject)

CX_DEF(cxPlayer, cxObject)
    cxInt channels;
    cxInt samples;
    cxInt index;
    cxHash caches;
    cxArray tracks;
    SLObjectItf SLESObject;
    SLEngineItf SLESEngine;
    SLObjectItf outputMixObject;
    SLEnvironmentalReverbItf reverbitf;
CX_END(cxPlayer, cxObject)

#endif

void cxPlayerSetEffectChannels(cxInt channels);

void cxPlayerSetEffectSamples(cxInt samples);

void cxPlayerInit(cxAny pthis);

void cxPlayerClear(cxAny pthis);

void cxPauseEffect(cxAny this);

void cxResumeEffect(cxAny this);

void cxStopEffect(cxAny this);

cxAny cxPlayEffect(cxConstChars file,cxFloat volume,cxBool loop);

void cxSetMusicVolume(cxFloat volume);

void cxPlayMusic(cxConstChars file,cxFloat volume,cxBool loop);

void cxStopMusic();

void cxPauseMusic();

void cxResumeMusic();

CX_C_END

#endif
