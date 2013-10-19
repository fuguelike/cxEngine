//
//  cxAppDelegate.m
//  cxEngineMac
//
//  Created by xuhua on 10/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#import "cxAppDelegate.h"

@implementation cxAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}

@end

#include <core/cxUtil.h>

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

cxBool cxEngineInit(cxEngine engine)
{

    return true;
}

void cxEngineMain(cxEngine engine)
{

}

void cxEngineFree(cxEngine engine)
{
    
}