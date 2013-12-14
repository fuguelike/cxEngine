//
//  cxAndroid.c
//  android
//
//  Created by xuhua on 12/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <android/asset_manager_jni.h>
#include <unistd.h>
#include <core/cxString.h>
#include <android/log.h>
#include "cxAndroid.h"

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
    CX_ASSERT(file != NULL, "args error");
    *start = 0;
    *length = 0;
    AAsset* asset = AAssetManager_open(NULL, file, AASSET_MODE_UNKNOWN);
    if(asset == NULL){
        CX_ERROR("assert open file %s failed",file);
        return -1;
    }
    cxInt fd = AAsset_openFileDescriptor(asset, (off_t *)start, (off_t *)length);
    AAsset_close(asset);
    return fd;
}

cxString cxAssetsPath(cxConstChars file)
{
    return cxStringConstChars(file);
}

CX_OBJECT_INIT(cxAndroid, cxObject)
{
}
CX_OBJECT_FREE(cxAndroid, cxObject)
{
}
CX_OBJECT_TERM(cxAndroid, cxObject)