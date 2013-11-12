//
//  cxPlayer.h
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxPlayer_h
#define cxEngine_cxPlayer_h

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "cxBase.h"
#include "cxArray.h"
#include "cxString.h"

CX_C_BEGIN

typedef enum {
    cxAudioFileTypeNone,
    cxAudioFileTypeMp3,
    cxAudioFileTypeWav,
}cxAudioFileType;

CX_OBJECT_DEF(cxBuffer, cxObject)
    ALuint buffer;
    ALenum format;
    ALsizei freq;
CX_OBJECT_END(cxBuffer)

cxBuffer cxBufferCreate(cxString data,ALenum format,ALsizei freq);

CX_OBJECT_DEF(cxTrack, cxObject)
    ALsizei freq;
    ALuint format;
    ALuint source;
CX_OBJECT_END(cxTrack)

void cxTrackPause(cxTrack this);

void cxTrackResume(cxTrack this);

void cxTrackStop(cxTrack this);

cxAudioFileType cxAudioGetType(cxConstChars file);

cxTrack cxPlayBuffer(cxAny buffer,cxBool loop);

cxTrack cxPlayFile(cxConstChars file,cxBool loop);

#define INIT_TRACK 5

CX_OBJECT_DEF(cxPlayer, cxObject)
    cxArray tracks;
    cxHash caches;
    ALCdevice *device;
    ALCcontext *context;
    CX_SLOT_ALLOC(onMemory);
CX_OBJECT_END(cxPlayer)

void cxPlayerOpen(cxInt freq,cxInt format);

void cxPlayerDestroy();

cxPlayer cxPlayerInstance();

CX_C_END

#endif
