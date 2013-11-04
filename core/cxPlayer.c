//
//  cxPlayer.c
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <mpg123.h>
#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxMp3Stream.h>
#include "cxPlayer.h"

CX_OBJECT_DEF(cxBuffer, cxObject)
    ALuint buffer;
    ALenum format;
    ALsizei freq;
CX_OBJECT_END(cxBuffer)

CX_OBJECT_INIT(cxBuffer, cxObject)
{
    alGenBuffers(1, &this->buffer);
}
CX_OBJECT_FREE(cxBuffer, cxObject)
{
    alDeleteBuffers(1, &this->buffer);
}
CX_OBJECT_TERM(cxBuffer, cxObject)

static cxBuffer cxBufferCreate(cxString data,ALenum format,ALsizei freq)
{
    cxBuffer this = CX_CREATE(cxBuffer);
    this->format = format;
    this->freq = freq;
    alBufferData(this->buffer, format, cxStringBody(data), cxStringLength(data), freq);
    return this;
}

static cxInt cxPlayerFreq = 22050;

static cxInt cxPlayerFormat = AL_FORMAT_MONO16;

cxAudioFileType cxAudioGetType(cxConstChars file)
{
    cxConstChars ext = strrchr(file, '.');
    CX_RETURN(ext == NULL,cxAudioFileTypeNone);
    if(cxConstCharsEqu(ext, ".mp3")){
        return cxAudioFileTypeMp3;
    }
    if(cxConstCharsEqu(ext, ".wav")){
        return cxAudioFileTypeWav;
    }
    return cxAudioFileTypeNone;
}

CX_OBJECT_INIT(cxTrack, cxObject)
{
    this->freq = cxPlayerFreq;
    this->format = cxPlayerFormat;
    alGenSources(1, &this->source);
}
CX_OBJECT_FREE(cxTrack, cxObject)
{
    alSourceStop(this->source);
    alDeleteSources(1, &this->source);
}
CX_OBJECT_TERM(cxTrack, cxObject)

void cxTrackPause(cxTrack this)
{
    alSourcePause(this->source);
}

void cxTrackResume(cxTrack this)
{
    alSourcePlay(this->source);
}

void cxTrackStop(cxTrack this)
{
    alSourceStop(this->source);
}

static cxPlayer instance = NULL;

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

cxTrack cxPlayBuffer(cxAny buffer,cxBool loop)
{
    cxBuffer cb = buffer;
    cxTrack track = cxPlayerQueryTrack();
    CX_RETURN(track == NULL || cb == NULL, NULL);
    alSourcei(track->source, AL_LOOPING, loop);
    alSourcei(track->source, AL_BUFFER, cb->buffer);
    alSourcePlay(track->source);
    return track;
}

cxTrack cxPlayFile(cxConstChars file,cxBool loop)
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
    if(type == cxAudioFileTypeWav){
        bytes = cxWAVSamples(file,&format,&freq);
    }else if(type == cxAudioFileTypeMp3){
        bytes = cxMP3Samples(file,&format,&freq);
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
    cxHashClean(this->caches);
}

CX_OBJECT_INIT(cxPlayer, cxObject)
{
    cxEngine engine = cxEngineInstance();
    CX_SLOT_QUICK(engine->onMemory, this, onMemory, cxPlayerOnMemory);
    
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
    mpg123_init();
}
CX_OBJECT_FREE(cxPlayer, cxObject)
{
    mpg123_exit();
    CX_RELEASE(this->caches);
    CX_RELEASE(this->tracks);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(this->context);
    alcCloseDevice(this->device);
    CX_SLOT_RELEASE(this->onMemory);
}
CX_OBJECT_TERM(cxPlayer, cxObject)

void cxPlayerOpen(cxInt freq,cxInt format)
{
    if(freq != 0){
        cxPlayerFreq = freq;
    }
    if(format != 0){
        cxPlayerFormat = format;
    }
    cxPlayerInstance();
}

cxPlayer cxPlayerInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxPlayer);
    }
    return instance;
}

void cxPlayerDestroy()
{
    CX_RELEASE(instance);
    instance = NULL;
}