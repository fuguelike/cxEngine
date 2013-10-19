//
//  cxUnit.m
//  cxEngineMac
//
//  Created by xuhua on 10/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//


#include <core/cxEngine.h>

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    NSLog(@"[%s:%d] %s:%s\n",file,line,type,buffer);
}

cxString cxAssetsPath(cxConstChars file)
{
    NSString *path = [[NSBundle mainBundle] resourcePath];
    if(file != NULL){
        return cxStringCreate("%s/%s",[path UTF8String],file);
    }else{
        return cxStringCreate("%s",[path UTF8String]);
    }
}
