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
#include <core/cxBase.h>

CX_C_BEGIN

typedef struct JniMethodInfo_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;

cxBool cxGetStaticMethodInfo(JniMethodInfo *methodinfo,cxConstChars className,cxConstChars methodName,cxConstChars paramCode);

AAssetManager *cxEngineGetAssetManager();

cxString jstringTocxString(jstring jstr);

CX_C_END

#endif
