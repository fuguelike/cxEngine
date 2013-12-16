//
//  cxAndroid.c
//  android
//
//  Created by xuhua on 12/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxString.h>
#include <android/log.h>
#include "cxAndroid.h"
#include <core/cxEngine.h>

static JNIEnv *javaENV = NULL;
static jclass javaClass = NULL;

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars font,cxTextAttr attr)
{
    CX_ASSERT(false, "not finished");
    return NULL;
}

cxString cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq)
{
    CX_ASSERT(false, "android platform not use");
    return NULL;
}

//cxEngineDocumentPath
cxString cxDocumentPath(cxConstChars file)
{
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineDocumentPath","(Ljava/lang/String;)Ljava/lang/String;");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    jstring path = NULL;
    if(file != NULL){
        path = (*methodInfo.env)->NewStringUTF(methodInfo.env,file);
    }
    jstring docPath = (*methodInfo.env)->CallStaticObjectMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID, path);
    if(path != NULL){
        (*methodInfo.env)->DeleteLocalRef(methodInfo.env,path);
    }
    cxString rv = jstringTocxString(docPath);
    (*methodInfo.env)->DeleteLocalRef(methodInfo.env,docPath);
    return rv;
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
    jboolean copy = JNI_FALSE;
    cxConstChars chars = (*javaENV)->GetStringUTFChars(javaENV,jstr,&copy);
    cxString rv = cxStringConstChars(chars);
    (*javaENV)->ReleaseStringUTFChars(javaENV,jstr,chars);
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
    if(cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineAssertManager","()Landroid/content/res/AssetManager;")){
        jobject assetObject = (*methodInfo.env)->CallStaticObjectMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID);
        assetManager = AAssetManager_fromJava(javaENV, assetObject);
        (*methodInfo.env)->DeleteLocalRef(methodInfo.env,assetObject);
    }
    return assetManager;
}

void cxEngineSendJson(cxString json)
{
    CX_ASSERT(json != NULL, "json args error");
    JniMethodInfo methodInfo;
    if(cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineRecvJson","(Ljava/lang/String;)V")){
        (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID);
    }
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

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineSendJsonImp(JNIEnv * env, jclass class,jstring json)
{
    CX_ASSERT(json != NULL, "args error");
    cxString js = jstringTocxString(json);
    cxEngineRecvJson(js);
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
    cxEngineExit();
    (*javaENV)->DeleteGlobalRef(javaENV,javaClass);
    javaENV = NULL;
    javaClass = NULL;
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











