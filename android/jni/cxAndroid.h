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
#include <core/cxBase.h>

CX_C_BEGIN

#define CLASS_NAME "cn/chelper/cxengine/EngineGLView"

typedef struct JniMethodInfo_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;

cxBool cxGetStaticMethodInfo(JniMethodInfo *methodinfo,cxConstChars className,cxConstChars methodName,cxConstChars paramCode);

AAssetManager *cxEngineGetAssetManager();

cxString jstringTocxString(jstring jstr);

void cxEngineRunJson(cxString json);

CX_C_END

#endif
