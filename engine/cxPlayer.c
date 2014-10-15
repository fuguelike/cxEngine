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

cxString cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq);

CX_OBJECT_DEF(cxBuffer, cxObject)
    ALuint buffer;
    ALenum format;
    ALsizei freq;
CX_OBJECT_END(cxBuffer, cxObject)

CX_OBJECT_TYPE(cxBuffer, cxObject)
{
    
}
CX_OBJECT_INIT(cxBuffer, cxObject)
{
    alGenBuffers(1, &this->buffer);
}
CX_OBJECT_FREE(cxBuffer, cxObject)
{
    alDeleteBuffers(1, &this->buffer);
}
CX_OBJECT_TERM(cxBuffer, cxObject)

static cxAny cxBufferCreate(cxString data,cxInt format,cxInt freq)
{
    cxBuffer this = CX_CREATE(cxBuffer);
    this->format = format;
    this->freq = freq;
    alBufferData(this->buffer, format, cxStringBody(data), cxStringLength(data), freq);
    return this;
}

CX_OBJECT_DEF(cxTrack, cxObject)
    ALsizei freq;
    ALuint format;
    ALuint source;
CX_OBJECT_END(cxTrack, cxObject)

CX_OBJECT_TYPE(cxTrack, cxObject)
{
    
}
CX_OBJECT_INIT(cxTrack, cxObject)
{
    alGenSources(1, &this->source);
}
CX_OBJECT_FREE(cxTrack, cxObject)
{
    alSourceStop(this->source);
    alDeleteSources(1, &this->source);
}
CX_OBJECT_TERM(cxTrack, cxObject)

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

static cxAny cxPlayBuffer(cxAny buffer,cxBool loop)
{
    cxBuffer cb = buffer;
    cxTrack track = cxPlayerQueryTrack();
    CX_RETURN(track == NULL || cb == NULL, NULL);
    alSourcei(track->source, AL_LOOPING, loop);
    alSourcei(track->source, AL_BUFFER, cb->buffer);
    alSourcePlay(track->source);
    return track;
}

cxAny cxPlayEffect(cxConstChars file,cxBool loop)
{
    cxPlayer this = cxPlayerInstance();
    cxBuffer buffer = cxHashGet(this->caches, cxHashStrKey(file));
    if(buffer != NULL){
        return cxPlayBuffer(buffer,loop);
    }
    cxAudioFileType type = cxAudioGetType(file);
    if(type == cxAudioFileTypeNone){
        CX_ERROR("not support play %s",file);
        return NULL;
    }
    cxUInt format = 0;
    cxUInt freq = 0;
    cxString bytes = NULL;
    if(type == cxAudioFileTypeWAV){
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
    return cxPlayBuffer(buffer,loop);
}

static void cxPlayerOnMemory(cxAny player)
{
    cxPlayer this = player;
    cxHashClear(this->caches);
}

CX_OBJECT_TYPE(cxPlayer, cxObject)
{
    
}
CX_OBJECT_INIT(cxPlayer, cxObject)
{
    cxEngine engine = cxEngineInstance();
    CX_CON(cxEngine, engine, onMemory, this, cxPlayerOnMemory);
    
    this->device = alcOpenDevice(NULL);
    CX_ASSERT(this->device != NULL, "alc open device failed");
    
    this->context = alcCreateContext(this->device, NULL);
    CX_ASSERT(this->context != NULL, "alc create context failed");
    
    alcMakeContextCurrent(this->context);    
    this->tracks = CX_ALLOC(cxArray);
    this->caches = CX_ALLOC(cxHash);
    for(int i=0; i < INIT_TRACK; i++){
        cxTrack track = CX_ALLOC(cxTrack);
        cxArrayAppend(this->tracks, track);
        CX_RELEASE(track);
    }
}
CX_OBJECT_FREE(cxPlayer, cxObject)
{
    CX_RELEASE(this->caches);
    CX_RELEASE(this->tracks);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(this->context);
    alcCloseDevice(this->device);
    CX_SLOT_RELEASE(this->onMemory);
}
CX_OBJECT_TERM(cxPlayer, cxObject)
