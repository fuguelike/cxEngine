//
//  cxPlayer.c
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxPlayer.h>
#include <engine/cxEngine.h>
#include <cxcore/cxBase.h>
#include "cxAndroid.h"

static cxInt effectChannels = 1;
/*
 #define SL_SAMPLINGRATE_8		((SLuint32) 8000000)
 #define SL_SAMPLINGRATE_11_025	((SLuint32) 11025000)
 #define SL_SAMPLINGRATE_12		((SLuint32) 12000000)
 #define SL_SAMPLINGRATE_16		((SLuint32) 16000000)
 #define SL_SAMPLINGRATE_22_05	((SLuint32) 22050000)
 #define SL_SAMPLINGRATE_24		((SLuint32) 24000000)
 #define SL_SAMPLINGRATE_32		((SLuint32) 32000000)
 #define SL_SAMPLINGRATE_44_1	((SLuint32) 44100000)
 */
static cxInt effectSamples = 8000;

void cxPlayerSetEffectChannels(cxInt channels)
{
    effectChannels = channels;
}

void cxPlayerSetEffectSamples(cxInt samples)
{
    effectSamples = samples;
}

//assert music player
static SLObjectItf     fdPlayerObject = NULL;
static SLPlayItf       fdPlayerPlay = NULL;
static SLSeekItf       fdPlayerSeek = NULL;
static SLMuteSoloItf   fdPlayerMuteSolo = NULL;
static SLVolumeItf     fdPlayerVolume = NULL;
//
static const SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

static void DestryAssertPlayer()
{
    if(fdPlayerPlay != NULL){
        (*fdPlayerPlay)->SetPlayState(fdPlayerPlay,SL_PLAYSTATE_STOPPED);
        fdPlayerPlay = NULL;
    }
    if(fdPlayerObject != NULL){
        (*fdPlayerObject)->Destroy(fdPlayerObject);
        fdPlayerObject = NULL;
        fdPlayerSeek = NULL;
        fdPlayerMuteSolo = NULL;
        fdPlayerVolume = NULL;
    }
}

CX_TYPE(cxBuffer, cxObject)
{
}
CX_INIT(cxBuffer, cxObject)
{
}
CX_FREE(cxBuffer, cxObject)
{
    CX_RELEASE(this->data);
}
CX_TERM(cxBuffer, cxObject)

CX_TYPE(cxTrack, cxObject)
{
}
CX_INIT(cxTrack, cxObject)
{
    
}
CX_FREE(cxTrack, cxObject)
{
    if(this->bqPlayerObject != NULL){
        (*this->bqPlayerObject)->Destroy(this->bqPlayerObject);
    }
}
CX_TERM(cxTrack, cxObject)

static cxInt GetVolumeLevel(cxFloat volume)
{
    int dbVolume = 2000 * log10f(volume);
    if(dbVolume < SL_MILLIBEL_MIN){
        dbVolume = SL_MILLIBEL_MIN;
    }
    if(dbVolume > SL_MILLIBEL_MAX){
        dbVolume = SL_MILLIBEL_MAX;
    }
    return dbVolume;
}

static void cxTrackSetVolume(cxTrack track,cxFloat volume)
{
    cxInt level = GetVolumeLevel(volume);
    (*track->bqPlayerVolume)->SetVolumeLevel(track->bqPlayerVolume,0);
}

static void cxTrackPlayBuffer(cxTrack track,cxBuffer buffer)
{
    cxAny buf = cxStrBody(buffer->data);
    cxInt siz = cxStrLength(buffer->data);
    (*track->bqPlayerBufferQueue)->Enqueue(track->bqPlayerBufferQueue,buf,siz);
}

static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
//    CX_ASSERT_THIS(context, cxTrack);
//    SLAndroidSimpleBufferQueueState state;
//    (*bq)->GetState(bq,&state);
}
static cxTrack cxTrackCreate(cxPlayer player)
{
    cxTrack this = CX_CREATE(cxTrack);
    SLresult result;
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 8};
    SLDataFormat_PCM format_pcm = {
        SL_DATAFORMAT_PCM,
        1,
        SL_SAMPLINGRATE_8,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER,
        SL_BYTEORDER_LITTLEENDIAN
    };
    //set channels and samples
    format_pcm.numChannels = effectChannels;
    format_pcm.samplesPerSec = effectSamples * 1000;
    //
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};
    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, player->outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};
    // create audio player
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,SL_BOOLEAN_TRUE};
    result = (*player->SLESEngine)->CreateAudioPlayer(player->SLESEngine, &this->bqPlayerObject, &audioSrc, &audioSnk,3, ids, req);
    CX_ASSERT(result == SL_RESULT_SUCCESS, "Create track Audio Player error");
    
    // realize the player
    result = (*this->bqPlayerObject)->Realize(this->bqPlayerObject, SL_BOOLEAN_FALSE);
    CX_ASSERT(result == SL_RESULT_SUCCESS, "Realize error");
    
    // get the play interface
    result = (*this->bqPlayerObject)->GetInterface(this->bqPlayerObject, SL_IID_PLAY, &this->bqPlayerPlay);
    CX_ASSERT(result == SL_RESULT_SUCCESS, "GetInterface SL_IID_PLAY error");
    
    // get the buffer queue interface
    result = (*this->bqPlayerObject)->GetInterface(this->bqPlayerObject, SL_IID_BUFFERQUEUE,&this->bqPlayerBufferQueue);
    CX_ASSERT(result == SL_RESULT_SUCCESS, "GetInterface SL_IID_BUFFERQUEUE error");
    
    // register callback on the buffer queue
    result = (*this->bqPlayerBufferQueue)->RegisterCallback(this->bqPlayerBufferQueue, bqPlayerCallback, this);
    CX_ASSERT(result == SL_RESULT_SUCCESS, "RegisterCallback error");
    
    // get the effect send interface
    result = (*this->bqPlayerObject)->GetInterface(this->bqPlayerObject, SL_IID_EFFECTSEND,&this->bqPlayerEffectSend);
    CX_ASSERT(result == SL_RESULT_SUCCESS, "GetInterface SL_IID_EFFECTSEND error");
    
    cxStopEffect(this);
    return this;
}

void cxPlayerInit(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxPlayer);
    SLresult result;
    // create engine
    result = slCreateEngine(&this->SLESObject, 0, NULL, 0, NULL, NULL);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"slCreateEngine error");
    result = (*this->SLESObject)->Realize(this->SLESObject, SL_BOOLEAN_FALSE);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"Realize error");
    //
    result = (*this->SLESObject)->GetInterface(this->SLESObject, SL_IID_ENGINE, &this->SLESEngine);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"GetInterface error");
    //
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*this->SLESEngine)->CreateOutputMix(this->SLESEngine, &this->outputMixObject, 1, ids, req);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"CreateOutputMix error");
    
    result = (*this->outputMixObject)->Realize(this->outputMixObject, SL_BOOLEAN_FALSE);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"Realize error");
    
    //the required MODIFY_AUDIO_SETTINGS permission was not requested and granted
    result = (*this->outputMixObject)->GetInterface(this->outputMixObject, SL_IID_ENVIRONMENTALREVERB,&this->reverbitf);
    if(SL_RESULT_SUCCESS == result) {
        (*this->reverbitf)->SetEnvironmentalReverbProperties(this->reverbitf, &reverbSettings);
    }
    for(cxInt i=0; i< INIT_TRACK;i++){
        cxTrack track = cxTrackCreate(this);
        cxArrayAppend(this->tracks, track);
    }
}

static cxBool cxPlayerMemory(cxAny pthis,cxAny engine)
{
    CX_ASSERT_THIS(pthis, cxPlayer);
    cxPlayerClear(this);
    return false;
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
    DestryAssertPlayer();
    CX_RELEASE(this->caches);
    CX_RELEASE(this->tracks);
    if (this->outputMixObject != NULL) {
        (*this->outputMixObject)->Destroy(this->outputMixObject);
    }
    if (this->SLESObject != NULL) {
        (*this->SLESObject)->Destroy(this->SLESObject);
    }
}
CX_TERM(cxPlayer, cxObject)

void cxPlayerClear(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxPlayer);
    CX_ARRAY_FOREACH(this->tracks, e){
        cxTrack track = cxArrayObject(e);
        cxStopEffect(track);
    }
    cxHashClear(this->caches);
}

static cxStr cxWAVSamplesWithData(cxStr data)
{
    return data;
}

cxAny cxBufferCreate(cxStr data)
{
    cxBuffer this = CX_CREATE(cxBuffer);
    data = cxWAVSamplesWithData(data);
    CX_RETAIN_SWAP(this->data, data);
    return this;
}

void cxPauseEffect(cxAny track)
{
    CX_ASSERT_THIS(track, cxTrack);
    (*this->bqPlayerPlay)->SetPlayState(this->bqPlayerPlay,SL_PLAYSTATE_PAUSED);
}

void cxResumeEffect(cxAny track)
{
    CX_ASSERT_THIS(track, cxTrack);
    (*this->bqPlayerPlay)->SetPlayState(this->bqPlayerPlay,SL_PLAYSTATE_PLAYING);
}

void cxStopEffect(cxAny track)
{
    CX_ASSERT_THIS(track, cxTrack);
    (*this->bqPlayerPlay)->SetPlayState(this->bqPlayerPlay,SL_PLAYSTATE_STOPPED);
    (*this->bqPlayerBufferQueue)->Clear(this->bqPlayerBufferQueue);
}

//return track
cxAny cxPlayEffect(cxConstChars file,cxFloat volume,cxBool loop)
{
    cxPlayer player = cxPlayerInstance();
    cxBuffer buffer = cxHashGet(player->caches, cxHashStrKey(file));
    if(buffer == NULL){
        cxStr data = cxAssetsData(file);
        buffer = cxBufferCreate(data);
        cxHashSet(player->caches, cxHashStrKey(file), buffer);
    }
    cxTrack track = cxArrayAtIndex(player->tracks, player->index++);
    if(player->index >= INIT_TRACK){
        player->index = 0;
    }
    cxTrackSetVolume(track, volume);
    cxTrackPlayBuffer(track, buffer);
    cxResumeEffect(track);
    return track;
}

void cxPlayMusic(cxConstChars path, cxFloat volume, cxBool loop)
{
    cxPlayer player = cxPlayerInstance();
    CX_ASSERT(cxConstCharsOK(path), "music path error");
    SLresult result;
    DestryAssertPlayer();
    AAssetManager* mgr = cxEngineGetAssetManager();
    CX_ASSERT(mgr != NULL, "cxEngineGetAssetManager NULL");
    //open file fd
    cxInt start, length;
    cxInt fd = cxAssertsFD(path, &start, &length);
    if(fd < 0){
        CX_ERROR("get assets fd error:%s",path);
        return;
    }
    // configure audio source
    SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_fd, &format_mime};
    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, player->outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};
    // create audio player
    const SLInterfaceID ids[4] = {SL_IID_PLAY, SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
    const SLboolean req[4] = {SL_BOOLEAN_TRUE,SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    //
    result = (*player->SLESEngine)->CreateAudioPlayer(player->SLESEngine, &fdPlayerObject, &audioSrc, &audioSnk,4, ids, req);
    CX_ASSERT(SL_RESULT_SUCCESS == result, "CreateAudioPlayer error");
    // realize the player
    result = (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"Realize error");
    // get the play interface
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY, &fdPlayerPlay);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"GetInterface SL_IID_PLAY error");
    // get the seek interface
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_SEEK, &fdPlayerSeek);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"GetInterface SL_IID_PLAY error");
    // get the mute/solo interface
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_MUTESOLO, &fdPlayerMuteSolo);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"GetInterface SL_IID_MUTESOLO error");
    // get the volume interface
    result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_VOLUME, &fdPlayerVolume);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"GetInterface SL_IID_VOLUME error");
    cxInt level = GetVolumeLevel(volume);
    (*fdPlayerVolume)->SetVolumeLevel(fdPlayerVolume,level);
    // enable whole file looping
    result = (*fdPlayerSeek)->SetLoop(fdPlayerSeek, loop, 0, SL_TIME_UNKNOWN);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"SetLoop error");
    result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay,SL_PLAYSTATE_PLAYING);
    CX_ASSERT(SL_RESULT_SUCCESS == result,"SetPlayState error");
}

void cxSetMusicVolume(cxFloat volume)
{
    CX_RETURN(fdPlayerPlay == NULL);
    cxInt level = GetVolumeLevel(volume);
    (*fdPlayerVolume)->SetVolumeLevel(fdPlayerVolume,level);
}

void cxStopMusic()
{
    CX_RETURN(fdPlayerPlay == NULL);
    (*fdPlayerPlay)->SetPlayState(fdPlayerPlay,SL_PLAYSTATE_STOPPED);
}

void cxPauseMusic()
{
    CX_RETURN(fdPlayerPlay == NULL);
    (*fdPlayerPlay)->SetPlayState(fdPlayerPlay,SL_PLAYSTATE_PAUSED);
}

void cxResumeMusic()
{
    CX_RETURN(fdPlayerPlay == NULL);
    (*fdPlayerPlay)->SetPlayState(fdPlayerPlay,SL_PLAYSTATE_PLAYING);
}






