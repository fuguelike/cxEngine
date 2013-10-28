//
//  cxPlayer.c
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include <streams/cxAssetsStream.h>
#include "cxPlayer.h"

CX_OBJECT_INIT(cxTrack, cxObject)
{
    this->freq = 11025;
    this->format = AL_FORMAT_MONO16;
    alGenBuffers(1, &this->buffer);
    alGenSources(1, &this->source);
}
CX_OBJECT_FREE(cxTrack, cxObject)
{
    alSourceStop(this->source);
    alDeleteBuffers(1, &this->buffer);
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

cxTrack cxPlayBuffer(cxString buffer,cxBool loop)
{
    cxTrack track = cxPlayerQueryTrack();
    CX_RETURN(track == NULL, NULL);
    alBufferData(track->buffer, track->format, cxStringBody(buffer), cxStringLength(buffer), track->freq);
    alSourcei(track->source, AL_LOOPING, loop);
    alSourcei(track->source, AL_BUFFER, track->buffer);
    alSourcePlay(track->source);
    return track;
}

cxTrack cxPlayFile(cxConstChars file,cxBool loop)
{
    cxPlayer this = cxPlayerInstance();
    cxString data = cxHashGet(this->caches, cxHashStrKey(file));
    if(data != NULL){
        goto completed;
    }
    data = cxWAVSamples(file);
    if(data == NULL){
        CX_ERROR("play wav file %s error",file);
        return NULL;
    }
    cxHashSet(this->caches, cxHashStrKey(file), data);
completed:
    return cxPlayBuffer(data,loop);
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

void cxPlayerOpen()
{
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