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
