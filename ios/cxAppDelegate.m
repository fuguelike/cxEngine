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

static void cxDisableDocumentBackup()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docPath = [paths objectAtIndex:0];
    NSURL *url = [NSURL fileURLWithPath:docPath];
    NSError *error = NULL;
    [url setResourceValue:[NSNumber numberWithBool:YES] forKey:NSURLIsExcludedFromBackupKey error:&error];
    if(error != NULL){
        NSLog(@"%@",error);
    }
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    cxDisableDocumentBackup();
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    self.window.contentScaleFactor = [UIScreen mainScreen].scale;
    cxGLViewController *glController = [[cxGLViewController alloc] init];
    self.rootViewController = glController;
    [self.window setRootViewController:glController];
    [self.window makeKeyAndVisible];
    [[glController getGLView] startMainLoop];
    [glController release];
    return YES;
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
    CX_LOGGER("%s play finished",cxStrBody(currFile));
}

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error
{
    CX_ERROR("pay music file %s decode error",cxStrBody(currFile));
}

-(void)cxPlayMusic:(cxConstChars)file volume:(cxFloat)volume loop:(cxBool)loop
{
    //save curr file path
    [currPlayer stop];
    [currPlayer release];
    CX_RETAIN_SWAP(currFile, cxStrConstChars(file));
    
    cxStr path = cxAssetsPath(file);
    CX_ASSERT(path != NULL, "path error");
    
    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:cxStrBody(path)]];
    NSError *error = nil;
    currPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
    currPlayer.delegate = self;
    currPlayer.volume = volume;
    CX_ASSERT(error == nil, "init audio player error %s,file=%s",[[error localizedDescription] UTF8String],file);
    [currPlayer setNumberOfLoops:loop ? -1 : 1];
    [currPlayer play];
    [url release];
}

-(void)cxSetMusicVolume:(float)volume
{
    if(currPlayer != nil)currPlayer.volume = volume;
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
    cxEAGLView *glView = [self.rootViewController getGLView];
    [glView pauseMainLoop];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    cxEAGLView *glView = [self.rootViewController getGLView];
    [glView resumeMainLoop];
}

-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    cxEAGLView *glView = [self.rootViewController getGLView];
    [glView memoryWarning];
}

@end
