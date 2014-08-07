//
//  cxAppDelegate.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <sys/xattr.h>
#include <engine/cxUtil.h>
#import "cxAppDelegate.h"
#import "cxEAGLView.h"

@implementation cxAppDelegate

static cxBool cxDisableDocumentBackup()
{
    cxString docPath = cxDocumentPath(NULL);
    u_int8_t attrValue = 0;
    cxConstChars path = cxStringBody(docPath);
    cxConstChars name = "com.apple.MobileBackup";
    if(getxattr(path, name, &attrValue, sizeof(attrValue), 0, 0) && attrValue == 1){
        return true;
    }
    attrValue = 1;
    return setxattr(path, name, &attrValue, sizeof(attrValue), 0, 0);
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    cxDisableDocumentBackup();
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    self.window.contentScaleFactor = [UIScreen mainScreen].scale;
    self.glViewController = [[[cxGLViewController alloc] init] autorelease];
    [self.window setRootViewController:self.glViewController];
    [self.window makeKeyAndVisible];
    //startup draw
    [[self.glViewController getGLView] startMainLoop];
    return YES;
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
    CX_LOGGER("%s play finished",cxStringBody(currFile));
}

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error
{
    CX_ERROR("pay music file %s decode error",cxStringBody(currFile));
}

-(void)cxPlayMusic:(cxConstChars)file loop:(cxBool)loop
{
    //save curr file path
    [currPlayer stop];
    [currPlayer release];
    CX_RETAIN_SWAP(currFile, cxStringConstChars(file));
    
    cxString path = cxAssetsPath(file);
    CX_ASSERT(path != NULL, "path error");
    
    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:cxStringBody(path)]];
    NSError *error = nil;
    currPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
    currPlayer.delegate = self;
    CX_ASSERT(error == nil, "init audio player error %s,file=%s",[[error localizedDescription] UTF8String],file);
    if(loop){
        [currPlayer setNumberOfLoops:-1];
    }
    [currPlayer play];
    [url release];
}

-(void)cxStopMusic
{
    [currPlayer stop];
    currPlayer = nil;
}

-(void)cxPauseMusic
{
    [currPlayer pause];
}

-(void)cxResumeMusic
{
    [currPlayer play];
}
							
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    cxEAGLView *glView = [self.glViewController getGLView];
    [glView pauseMainLoop];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    cxEAGLView *glView = [self.glViewController getGLView];
    [glView resumeMainLoop];
}

-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    cxEAGLView *glView = [self.glViewController getGLView];
    [glView memoryWarning];
}

@end
