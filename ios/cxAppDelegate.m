//
//  cxAppDelegate.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <sys/xattr.h>
#include <core/cxUtil.h>
#import "cxAppDelegate.h"
#import "cxEAGLView.h"

@implementation cxAppDelegate

static cxBool cxDisableDocumentBackup()
{
    cxString docPath = cxDocumentPath(NULL);
    cxConstChars filePath = cxStringBody(docPath);
    u_int8_t attrValue = 1;
    return setxattr(filePath, "com.apple.MobileBackup", &attrValue, sizeof(attrValue), 0, 0) == 0;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    //
    cxDisableDocumentBackup();
    //
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    self.window.contentScaleFactor = [UIScreen mainScreen].scale;
    //init gl view
    cxEAGLView *glView = [[cxEAGLView alloc] initWithFrame:[self.window bounds]];
    [glView setFrame:self.window.frame];
    [glView setContentScaleFactor:self.window.contentScaleFactor];
    [glView setMultipleTouchEnabled:NO];
    //
    self.viewController = [[[cxViewController alloc] init] autorelease];
    self.viewController.view = glView;
    [self.window setRootViewController:self.viewController];
    [glView release];
    //start run
    [glView startMainLoop];
    [self.window makeKeyAndVisible];
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
    CX_RETAIN_SWAP(currFile, cxStringConstChars(file));
    
    cxString path = cxAssetsPath(file);
    CX_ASSERT(path != NULL, "path error");
    
    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:cxStringBody(path)]];
    [currPlayer stop];
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
    cxEAGLView *glView = (cxEAGLView *)self.viewController.view;
    [glView pauseMainLoop];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    cxEAGLView *glView = (cxEAGLView *)self.viewController.view;
    [glView resumeMainLoop];
}

-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    cxEAGLView *glView = (cxEAGLView *)self.viewController.view;
    [glView memoryWarning];
}

@end
