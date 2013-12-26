//
//  cxPlayer.c
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxPlayer.h>
#include <core/cxHash.h>
#include "cxAndroid.h"
#include <core/cxPlayerLuaImp.c>

CX_OBJECT_DEF(cxTrack, cxObject)
    cxInt soundId;
    cxString file;
CX_OBJECT_END(cxTrack)

CX_OBJECT_INIT(cxTrack, cxObject)
{
    
}
CX_OBJECT_FREE(cxTrack, cxObject)
{
    CX_RELEASE(this->file);
}
CX_OBJECT_TERM(cxTrack, cxObject)

CX_OBJECT_INIT(cxPlayer, cxObject)
{
    this->tracks = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxPlayer, cxObject)
{
    CX_RELEASE(this->tracks);
}
CX_OBJECT_TERM(cxPlayer, cxObject)

static cxPlayer instance = NULL;

static cxInt cxPlayerFreq = 22050;

static cxInt cxPlayerFormat = 0;

cxAny cxBufferCreate(cxString data,cxInt format,cxInt freq)
{
    CX_ASSERT(false, "cxBufferCreate not use at android");
    return NULL;
}

void cxPauseEffect(cxAny track)
{
    cxTrack this = track;
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEnginePauseEffect","(I)V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID, this->soundId);
}

void cxResumeEffect(cxAny track)
{
    cxTrack this = track;
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineResumeEffect","(I)V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID, this->soundId);
}

void cxStopEffect(cxAny track)
{
    cxTrack this = track;
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineStopEffect","(I)V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID, this->soundId);
}

cxAny cxPlayBuffer(cxAny buffer,cxBool loop)
{
    CX_ASSERT(false, "cxPlayBuffer not use at android");
    return NULL;
}

cxAny cxPlayEffect(cxConstChars file,cxBool loop)
{
    cxPlayer this = cxPlayerInstance();
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEnginePlayEffect","(Ljava/lang/String;Z)I");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    jstring path = (*methodInfo.env)->NewStringUTF(methodInfo.env,file);
    cxInt soundId = (*methodInfo.env)->CallStaticIntMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID, path, loop);
    (*methodInfo.env)->DeleteLocalRef(methodInfo.env,path);
    if(soundId <= 0){
        CX_ERROR("play file failed %s",file);
        return NULL;
    }
    cxTrack track = cxHashGet(this->tracks, cxHashStrKey(file));
    //add or replace
    if(track == NULL || track->soundId != soundId){
        track = CX_ALLOC(cxTrack);
        track->file = cxStringAllocChars(file);
        track->soundId = soundId;
        cxHashSet(this->tracks, cxHashStrKey(file), track);
        CX_RELEASE(track);
    }
    return track;
}

void cxPlayMusic(cxConstChars file,cxBool loop){
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEnginePlayMusic","(Ljava/lang/String;Z)V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    jstring path = (*methodInfo.env)->NewStringUTF(methodInfo.env,file);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID, path, loop);
    (*methodInfo.env)->DeleteLocalRef(methodInfo.env,path);
}

void cxStopMusic()
{
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineStopMusic","()V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID);
}

void cxPauseMusic()
{
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEnginePauseMusic","()V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID);
}

void cxResumeMusic()
{
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineResumeMusic","()V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID);
}

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

void cxPlayerDestroy()
{
    CX_RELEASE(instance);
    instance = NULL;
}

cxAny cxPlayerInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxPlayer);
    }
    return instance;
}






