//
//  cxAndroid.h
//  cxEngine
//
//  Created by xuhua on 12/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef android_cxAndroid_h
#define android_cxAndroid_h

#include <jni.h>
#include <android/input.h>
#include <android/asset_manager_jni.h>
#include <unistd.h>
#include <cxcore/cxBase.h>

CX_C_BEGIN

#define CLASS_NAME "com/cxengine/EngineGLView"

typedef struct {
    jmethodID methodID;
    cxBool isGet;
} JniMethodInfo;

extern JNIEnv *javaENV;
extern jclass javaClass;

#define CX_GET_METHOD(_v_,_n_,_d_)    \
if(!(_v_).isGet){ \
    cxGetStaticMethodInfo(&(_v_), CLASS_NAME, _n_,_d_);\
    CX_ASSERT((_v_).isGet,"get method "_n_" failed");\
}
#define M(_m_) javaENV, javaClass, (_m_).methodID

cxBool cxGetStaticMethodInfo(JniMethodInfo *methodinfo,cxConstChars className,cxConstChars methodName,cxConstChars paramCode);

AAssetManager *cxEngineGetAssetManager();

cxString jstringTocxString(jstring jstr);

jstring cxStringTojstring(cxString str);

void cxEngineRunJson(cxString json);

CX_C_END

#endif
