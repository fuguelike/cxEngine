//
//  cxPlayer.c
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <engine/cxUtil.h>
#include <streams/cxAssetsStream.h>
#include "cxPlayer.h"
#include "cxEngine.h"

static cxInt effectChannels = 1;

static cxInt effectSamples = 8000;

void cxPlayerSetEffectChannels(cxInt channels)
{
    effectChannels = channels;
}

void cxPlayerSetEffectSamples(cxInt samples)
{
    effectSamples = samples;
}

cxStr cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq);

CX_TYPE(cxBuffer, cxObject)
{
    
}
CX_INIT(cxBuffer, cxObject)
{
    alGenBuffers(1, &this->buffer);
}
CX_FREE(cxBuffer, cxObject)
{
    alDeleteBuffers(1, &this->buffer);
}
CX_TERM(cxBuffer, cxObject)

static cxAny cxBufferCreate(cxStr data,cxInt format,cxInt freq)
{
    cxBuffer this = CX_CREATE(cxBuffer);
    this->format = format;
    this->freq = freq;
    alBufferData(this->buffer, format, cxStrBody(data), cxStrLength(data), freq);
    return this;
}

CX_TYPE(cxTrack, cxObject)
{
    
}
CX_INIT(cxTrack, cxObject)
{
    alGenSources(1, &this->source);
}
CX_FREE(cxTrack, cxObject)
{
    alSourceStop(this->source);
    alDeleteSources(1, &this->source);
}
CX_TERM(cxTrack, cxObject)

void cxPauseEffect(cxAny track)
{
    cxTrack this = track;
    alSourcePause(this->source);
}

void cxResumeEffect(cxAny track)
{
    cxTrack this = track;
    alSourcePlay(this->source);
}

void cxStopEffect(cxAny track)
{
    cxTrack this = track;
    alSourceStop(this->source);
}

static cxTrack cxPlayerQueryTrack()
{
    cxPlayer this = cxPlayerInstance();
    cxTrack track = NULL;
    //from not use track
    CX_ARRAY_FOREACH(this->tracks, ele){
        track = cxArrayObject(ele);
        ALint state = 0;
        alGetSourcei(track->source, AL_SOURCE_STATE, &state);
        if(state == AL_INITIAL){
            return track;
        }
        if(state == AL_STOPPED){
            return track;
        }
    }
    //create new track
    track = CX_ALLOC(cxTrack);
    cxArrayAppend(this->tracks, track);
    CX_RELEASE(track);
    return track;
}

static cxAny cxPlayBuffer(cxAny buffer,cxFloat volume,cxBool loop)
{
    cxBuffer cb = buffer;
    cxTrack track = cxPlayerQueryTrack();
    CX_RETURN(track == NULL || cb == NULL, NULL);
    alSourcei(track->source, AL_LOOPING, loop);
    alSourcei(track->source, AL_BUFFER, cb->buffer);
    alSourcef(track->source, AL_GAIN, volume);
    alSourcePlay(track->source);
    return track;
}

cxAny cxPlayEffect(cxConstChars file,cxFloat volume,cxBool loop)
{
    cxPlayer this = cxPlayerInstance();
    cxBuffer buffer = cxHashGet(this->caches, cxHashStrKey(file));
    if(buffer != NULL){
        return cxPlayBuffer(buffer,volume,loop);
    }
    cxAudioFileType type = cxAudioGetType(file);
    if(type == cxAudioFileTypeNone){
        CX_ERROR("not support play %s",file);
        return NULL;
    }
    cxUInt format = 0;
    cxUInt freq = 0;
    cxStr bytes = NULL;
    if(type == cxAudioFileTypePCM){
        if(effectChannels == 2)format = AL_FORMAT_STEREO16;
        else format = AL_FORMAT_MONO16;
        freq = effectSamples;
        bytes = cxAssetsData(file);
    }else if(type == cxAudioFileTypeWAV){
        bytes = cxWAVSamplesWithFile(file,&format,&freq);
    }
    if(bytes == NULL){
        CX_ERROR("play wav file %s error",file);
        return NULL;
    }
    buffer = cxBufferCreate(bytes, format, freq);
    if(buffer == NULL){
        CX_ERROR("create track buffer error");
        return NULL;
    }
    cxHashSet(this->caches, cxHashStrKey(file), buffer);
    return cxPlayBuffer(buffer,volume,loop);
}

void cxPlayerClear(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxPlayer);
    CX_ARRAY_FOREACH(this->tracks, e){
        cxTrack track = cxArrayObject(e);
        cxStopEffect(track);
    }
    cxHashClear(this->caches);
}

static cxBool cxPlayerMemory(cxAny pthis,cxAny engine)
{
    CX_ASSERT_THIS(pthis, cxPlayer);
    cxPlayerClear(this);
    return false;
}

void cxPlayerInit(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxPlayer);
    this->device = alcOpenDevice(NULL);
    CX_ASSERT(this->device != NULL, "alc open device failed");
    this->context = alcCreateContext(this->device, NULL);
    CX_ASSERT(this->context != NULL, "alc create context failed");
    alcMakeContextCurrent(this->context);
    for(int i=0; i < INIT_TRACK; i++){
        cxTrack track = CX_ALLOC(cxTrack);
        cxArrayAppend(this->tracks, track);
        CX_RELEASE(track);
    }
}

CX_TYPE(cxPlayer, cxObject)
{
    
}
CX_INIT(cxPlayer, cxObject)
{
    this->tracks = CX_ALLOC(cxArray);
    this->caches = CX_ALLOC(cxHash);
    cxMessageAppend(this, cxPlayerMemory, cxEngineNoticMemory);
    cxPlayerInit(this);
}
CX_FREE(cxPlayer, cxObject)
{
    CX_RELEASE(this->caches);
    CX_RELEASE(this->tracks);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(this->context);
    alcCloseDevice(this->device);
}
CX_TERM(cxPlayer, cxObject)
