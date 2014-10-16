//
//  cxAndroid.c
//  android
//
//  Created by xuhua on 12/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <android/log.h>
#include "cxAndroid.h"
#include <engine/cxEngine.h>
#include <engine/cxGlobal.h>

JNIEnv *javaENV = NULL;
jclass javaClass = NULL;

JniMethodInfo createTextBitmap  = {.isGet=false};
JniMethodInfo engineTerminate   = {.isGet=false};
JniMethodInfo documentPath      = {.isGet=false};
JniMethodInfo engineLocalized   = {.isGet=false};
JniMethodInfo assertManager     = {.isGet=false};
JniMethodInfo engineRecvJson    = {.isGet=false};

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars font,cxFloat size,cxTextAlign align,cxInt cw,cxInt ch)
{
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "env and class error");
    CX_GET_METHOD(createTextBitmap,"createTextBitmap","(Ljava/lang/String;Ljava/lang/String;IIII)[B");
    cxString rv = NULL;
    jstring strText = (*javaENV)->NewStringUTF(javaENV,txt);
    jstring fontName = NULL;
    if(font != NULL){
        fontName = (*javaENV)->NewStringUTF(javaENV,font);
    }
    jbyteArray bytes = (jbyteArray)(*javaENV)->CallStaticObjectMethod(M(createTextBitmap),strText,fontName,(jint)size,(jint)align,(jint)cw,(jint)ch);
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
    if(strText != NULL){
        (*javaENV)->DeleteLocalRef(javaENV,strText);
    }
    return rv;
}
//cxEngineTerminate
void cxEngineTerminate()
{
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "env and class error");
    CX_GET_METHOD(engineTerminate,"cxEngineTerminate","()V");
    (*javaENV)->CallStaticVoidMethod(M(engineTerminate));
    (*javaENV)->DeleteGlobalRef(javaENV,javaClass);
    javaENV = NULL;
    javaClass = NULL;
    cxGlobalFree();
}

//java:String cxEngineDocumentPath()
cxString cxDocumentPath(cxConstChars file)
{
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "env and class error");
    CX_GET_METHOD(documentPath,"cxEngineDocumentPath","(Ljava/lang/String;)Ljava/lang/String;");
    jstring path = NULL;
    if(file != NULL){
        path = (*javaENV)->NewStringUTF(javaENV,file);
    }
    jstring docPath = (*javaENV)->CallStaticObjectMethod(M(documentPath), path);
    cxString rv = jstringTocxString(docPath);
    if(docPath != NULL){
        (*javaENV)->DeleteLocalRef(javaENV,docPath);
    }
    if(path != NULL){
        (*javaENV)->DeleteLocalRef(javaENV,path);
    }
    return rv;
}

//java:String cxEngineLocalized()
cxString cxLocalizedLang()
{
    CX_ASSERT(javaENV != NULL && javaClass != NULL, "env and class error");
    CX_GET_METHOD(engineLocalized,"cxEngineLocalized","()Ljava/lang/String;");
    cxString lngcode = NULL;
    jstring lang = (*javaENV)->CallStaticObjectMethod(M(engineLocalized));
    CX_ASSERT(lang != NULL, "get lang code failed");
    lngcode = jstringTocxString(lang);
    (*javaENV)->DeleteLocalRef(javaENV,lang);
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

cxInt cxAssertsFD(cxConstChars file,cxInt *off,cxInt *length)
{
    CX_ASSERT(off != NULL && length != NULL && file != NULL, "args error");
    cxString path = cxAssetsPath(file);
    AAssetManager *assetMgr = cxEngineGetAssetManager();
    AAsset *asset = AAssetManager_open(assetMgr, cxStringBody(path), AASSET_MODE_UNKNOWN);
    if(asset == NULL){
        return -1;
    }
    off_t outStart = 0;
    off_t outLength = 0;
    cxInt fd = AAsset_openFileDescriptor(asset,&outStart,&outLength);
    *off = (cxInt)outStart;
    *length = (cxInt)outLength;
    AAsset_close(asset);
    return fd;
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

jstring cxStringTojstring(cxString str)
{
    if(!cxStringOK(str)){
        return NULL;
    }
    CX_ASSERT(javaENV != NULL, "java env error");
    return (*javaENV)->NewStringUTF(javaENV,cxStringBody(str));
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
    methodinfo->isGet = true;
    methodinfo->methodID = methodID;
    return true;
}

AAssetManager *cxEngineGetAssetManager()
{
    static AAssetManager *assetManager = NULL;
    if(assetManager != NULL){
        return assetManager;
    }
    CX_GET_METHOD(assertManager,"cxEngineAssertManager","()Landroid/content/res/AssetManager;");
    jobject assetObject = (*javaENV)->CallStaticObjectMethod(M(assertManager));
    assetManager = AAssetManager_fromJava(javaENV, assetObject);
    (*javaENV)->DeleteLocalRef(javaENV,assetObject);
    return assetManager;
}

void cxEngineSendJson(cxString json)
{
    CX_ASSERT(javaENV != NULL && json != NULL, "json args error");
    CX_GET_METHOD(engineRecvJson,"cxEngineRecvJson","(Ljava/lang/String;)V");
    jstring str = cxStringTojstring(json);
    (*javaENV)->CallStaticVoidMethod(M(engineRecvJson),str);
    (*javaENV)->DeleteLocalRef(javaENV,str);
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEngineFireTouch(JNIEnv * env, jclass class,jint action,jintArray ids, jfloatArray xs, jfloatArray ys)
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
    //
    int size = (*env)->GetArrayLength(env,ids);
    jint id[size];
    jfloat x[size];
    jfloat y[size];
    (*env)->GetIntArrayRegion(env, ids, 0, size, id);
    (*env)->GetFloatArrayRegion(env, xs, 0, size, x);
    (*env)->GetFloatArrayRegion(env, ys, 0, size, y);
    //
    cxTouchPoint points[CX_MAX_TOUCH_POINT];
    cxInt num = 0;
    for (cxInt i=0; i < size; i++) {
        points[num].xy = cxVec2fv(x[i],y[i]);
        points[num].id = id[i];
        num ++;
    }
    cxEngineFireTouch(cxtype, num, points);
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEngineFireKey(JNIEnv * env, jclass class,jint type,jint code)
{
    cxKeyType cxtype = cxKeyTypeDown;
    if(type == AKEY_EVENT_ACTION_DOWN){
        cxtype = cxKeyTypeDown;
    }else if(type == AKEY_EVENT_ACTION_UP){
        cxtype = cxKeyTypeUp;
    }else if(type == AKEY_EVENT_ACTION_MULTIPLE){
        cxtype = cxKeyTypeMultiple;
    }else{
        CX_ASSERT(false, "fire key type error");
    }
    cxEngineFireKey(cxtype, code);
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEngineSendJsonImp(JNIEnv * env, jclass class,jstring json)
{
    CX_ASSERT(json != NULL, "args error");
    cxString js = jstringTocxString(json);
    cxEngineRecvJson(js);
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEngineMemory(JNIEnv * env, jclass class)
{
    cxEngineMemory();
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEngineResume(JNIEnv * env, jclass class)
{
    cxEngineResume();
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEnginePause(JNIEnv * env, jclass class)
{
    cxEnginePause();
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEngineDraw(JNIEnv * env, jclass class,jfloat dt)
{
    cxEngineDraw(dt);
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEngineLayout(JNIEnv * env, jclass class,  jint width, jint height)
{
    cxEngineLayout(width, height);
}

JNIEXPORT void JNICALL Java_com_cxengine_EngineGLView_cxEngineStartup(JNIEnv * env, jclass class)
{
    javaENV = env;
    if(javaClass != NULL){
        (*env)->DeleteGlobalRef(env,javaClass);
    }
    javaClass = (*env)->NewGlobalRef(env,class);
    cxEngineStartup();
}











