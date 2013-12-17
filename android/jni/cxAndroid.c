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
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "env and class error");
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "createTextBitmap","(Ljava/lang/String;Ljava/lang/String;I)[B");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    cxString rv = NULL;
    jstring str = (*javaENV)->NewStringUTF(javaENV,txt);
    jstring fontName = NULL;
    if(font != NULL){
        fontName = (*javaENV)->NewStringUTF(javaENV,font);
    }
    jbyteArray bytes = (jbyteArray)(*javaENV)->CallStaticObjectMethod(javaENV,methodInfo.classID,methodInfo.methodID,str,fontName,(jint)attr.size);
    if(bytes != NULL){
        jboolean ok = JNI_FALSE;
        jsize length = (*javaENV)->GetArrayLength(javaENV,bytes);
        jbyte *buffer = (*javaENV)->GetByteArrayElements(javaENV,bytes,&ok);
        rv = cxStringBinary(buffer, length);
        (*javaENV)->ReleaseByteArrayElements(javaENV,bytes,buffer,0);
    }
    if(fontName != NULL){
        (*javaENV)->DeleteLocalRef(javaENV,fontName);
    }
    if(str != NULL){
        (*javaENV)->DeleteLocalRef(javaENV,str);
    }
    return rv;
}
//cxEngineTerminate
void cxEngineTerminate()
{
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "env and class error");
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineTerminate","()V");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    (*methodInfo.env)->CallStaticVoidMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID);
}

//java:String cxEngineDocumentPath()
cxString cxDocumentPath(cxConstChars file)
{
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "env and class error");
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineDocumentPath","(Ljava/lang/String;)Ljava/lang/String;");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    jstring path = NULL;
    if(file != NULL){
        path = (*methodInfo.env)->NewStringUTF(methodInfo.env,file);
    }
    jstring docPath = (*methodInfo.env)->CallStaticObjectMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID, path);
    cxString rv = jstringTocxString(docPath);
    if(docPath != NULL){
        (*methodInfo.env)->DeleteLocalRef(methodInfo.env,docPath);
    }
    if(path != NULL){
        (*methodInfo.env)->DeleteLocalRef(methodInfo.env,path);
    }
    return rv;
}

//java:String cxEngineLocalized()
cxString cxLocalizedLang()
{
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "env and class error");
    JniMethodInfo methodInfo;
    cxBool ret = cxGetStaticMethodInfo(&methodInfo, CLASS_NAME, "cxEngineLocalized","()Ljava/lang/String;");
    CX_ASSERT(ret, "get static method info failed");
    CX_UNUSED_PARAM(ret);
    cxString lngcode = NULL;
    jstring lang = (*methodInfo.env)->CallStaticObjectMethod(methodInfo.env, methodInfo.classID, methodInfo.methodID);
    CX_ASSERT(lang != NULL, "get lang code failed");
    lngcode = jstringTocxString(lang);
    (*methodInfo.env)->DeleteLocalRef(methodInfo.env,lang);
    CX_LOGGER("current localized lang:%s",cxStringBody(lngcode));
    return lngcode;
}

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    if(cxConstCharsEqu(type, "ERROR") || cxConstCharsEqu(type, "ASSERT")){
        __android_log_print(ANDROID_LOG_ERROR, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else if(cxConstCharsEqu(type, "WARN")){
        __android_log_print(ANDROID_LOG_WARN, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }else{
        __android_log_print(ANDROID_LOG_INFO, "cxEngine", "[%s:%d] %s:%s",file,line,type,buffer);
    }
}

cxString cxAssetsPath(cxConstChars file)
{
    CX_RETURN(file == NULL, NULL);
    return cxStringConstChars(file);
}

cxBool cxAssetsExists(cxConstChars file)
{
    cxBool ret = false;
    cxString path = cxAssetsPath(file);
    AAssetManager *assetMgr = cxEngineGetAssetManager();
    AAsset *asset = AAssetManager_open(assetMgr, cxStringBody(path), AASSET_MODE_UNKNOWN);
    if(asset != NULL){
        ret = AAsset_getLength(asset) > 0;
        AAsset_close(asset);
    }
    return ret;
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

JNIEXPORT void JNICALL Java_cn_chelper_cxengine_EngineGLView_cxEngineBegin(JNIEnv * env, jclass class)
{
    javaENV = env;
    javaClass = (*env)->NewGlobalRef(env,class);
    cxEngineBegin();
}











