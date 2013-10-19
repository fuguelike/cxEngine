//
//  cxAppDelegate.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import "cxAppDelegate.h"
#import "cxEAGLView.h"

@implementation cxAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.contentScaleFactor = [UIScreen mainScreen].scale;
    //init gl view
    cxEAGLView *glView = [[cxEAGLView alloc] initWithFrame:[self.window bounds]];
    [glView setContentScaleFactor:self.window.contentScaleFactor];
    [glView setMultipleTouchEnabled:NO];
    //
    self.viewController = [[cxViewController alloc] init];
    self.viewController.wantsFullScreenLayout = YES;
    self.viewController.view = glView;
    [self.window setRootViewController:self.viewController];
    //start run
    [glView startMainLoop];
    [self.window makeKeyAndVisible];
    return YES;
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
