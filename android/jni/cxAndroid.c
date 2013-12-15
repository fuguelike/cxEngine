//
//  cxAndroid.c
//  android
//
//  Created by xuhua on 12/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <android/input.h>
#include <android/asset_manager_jni.h>
#include <unistd.h>
#include <core/cxString.h>
#include <android/log.h>
#include "cxAndroid.h"
#include <core/cxEngine.h>

static JNIEnv *javaENV = NULL;
static jclass javaClass = NULL;

cxAny cxBufferCreate(cxString data,cxInt format,cxInt freq)
{
    return NULL;
}

void cxTrackPause(cxAny track)
{
    
}

void cxTrackResume(cxAny track)
{
    
}

void cxTrackStop(cxAny track)
{
    
}

cxAny cxPlayBuffer(cxAny buffer,cxBool loop)
{
    return NULL;
}

cxAny cxPlayFile(cxConstChars file,cxBool loop)
{
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, "cn/chelper/cxengine/EngineGLView", "cxEnginePlayFile","(Ljava/lang/String;Z)V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    jstring path = (*methodInfo.env)->NewStringUTF(methodInfo.env,file);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID, path, loop);
    (*methodInfo.env)->DeleteLocalRef(methodInfo.env,path);
    return NULL;
}

void cxPlayerOpen(cxInt freq,cxInt format)
{
    
}

void cxPlayerDestroy()
{
    
}

cxAny cxPlayerInstance()
{
    return NULL;
}

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars font,cxTextAttr attr)
{
    return NULL;
}

cxString cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq)
{
    return NULL;
}

cxString cxDocumentPath(cxConstChars file)
{
    return NULL;
}

cxString cxLocaleLang()
{
    return UTF8("en");
}

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    if(cxConstCharsEqu(type, "ERROR") || cxConstCharsEqu(type, "ASSERT")){
        __android_log_print(ANDROID_LOG_ERROR, "cxEngine", "[%s:%d] %s:%s\n",file,line,type,buffer);
    }else if(cxConstCharsEqu(type, "WARN")){
        __android_log_print(ANDROID_LOG_WARN, "cxEngine", "[%s:%d] %s:%s\n",file,line,type,buffer);
    }else{
        __android_log_print(ANDROID_LOG_INFO, "cxEngine", "[%s:%d] %s:%s\n",file,line,type,buffer);
    }
}

cxInt cxAssertsOpen(cxConstChars file,cxInt *start,cxInt *length)
{
    CX_ASSERT(false, "android not support");
    return 0;
}

cxString cxAssetsPath(cxConstChars file)
{
    CX_RETURN(file == NULL, NULL);
    return cxStringConstChars(file);
}

cxString jstringTocxString(jstring jstr) {
    if (jstr == NULL) {
        return NULL;
    }
    CX_ASSERT(javaENV != NULL, "java env error");
    const jchar *chars = (*javaENV)->GetStringChars(javaENV, jstr,NULL);
    cxString rv = UTF8("%s",chars);
    (*javaENV)->ReleaseStringChars(javaENV,jstr,chars);
    return rv;
}

cxBool cxGetStaticMethodInfo(JniMethodInfo *methodinfo,cxConstChars className,cxConstChars methodName,cxConstChars paramCode)
{
    if ((NULL == className) || (NULL == methodName) || (NULL == paramCode)) {
        return false;
    }
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "java env and class error");
    
    jmethodID methodID = (*javaENV)->GetStaticMethodID(javaENV, javaClass, methodName, paramCode);
    if (methodID == NULL) {
        CX_ERROR("Failed to find static method id of %s", methodName);
        return false;
    }
    
    methodinfo->classID = javaClass;
    methodinfo->env = javaENV;
    methodinfo->methodID = methodID;
    return true;
}

AAssetManager *cxEngineGetAssetManager()
{
    static AAssetManager *assetManager = NULL;
    if(assetManager != NULL){
        return assetManager;
    }
    JniMethodInfo methodInfo;
    if(cxGetStaticMethodInfo(&methodInfo, "cn/chelper/cxengine/EngineGLView", "cxEngineAssertManager","()Landroid/content/res/AssetManager;")){
        jobject assetObject = (*methodInfo.env)->CallStaticObjectMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID);
        assetManager = AAssetManager_fromJava(javaENV, assetObject);
        (*methodInfo.env)->DeleteLocalRef(methodInfo.env,assetObject);
    }
    return assetManager;
}

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineFireTouch(JNIEnv * env, jclass class,jint action,jfloat x,jfloat y)
{
    cxTouchType cxtype = cxTouchTypeCancel;
    if(action == AMOTION_EVENT_ACTION_DOWN){
        cxtype = cxTouchTypeDown;
    }else if(action == AMOTION_EVENT_ACTION_MOVE){
        cxtype = cxTouchTypeMove;
    }else if(action == AMOTION_EVENT_ACTION_UP){
        cxtype = cxTouchTypeUp;
    }else if(action == AMOTION_EVENT_ACTION_CANCEL){
        cxtype = cxTouchTypeCancel;
    }else{
        CX_ASSERT(false, "touch action error %d",action);
    }
    cxEngineFireTouch(cxtype, cxVec2fv(x, y));
}

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineMemory(JNIEnv * env, jclass class)
{
    cxEngineMemory();
}

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineResume(JNIEnv * env, jclass class)
{
    cxEngineResume();
}

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEnginePause(JNIEnv * env, jclass class)
{
    cxEnginePause();
}

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineExit(JNIEnv * env, jclass class)
{
    (*javaENV)->DeleteGlobalRef(javaENV,javaClass);
    javaENV = NULL;
    javaClass = NULL;
    cxEngineExit();
}

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineDraw(JNIEnv * env, jclass class)
{
    cxEngineDraw();
}

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineLayout(JNIEnv * env, jclass class,  jint width, jint height)
{
    cxEngineLayout(width, height);
}

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineInit(JNIEnv * env, jclass class)
{
    javaENV = env;
    javaClass = (*env)->NewGlobalRef(env,class);
    cxEngineInit(cxEngineInstance());
}











