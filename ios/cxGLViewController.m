//
//  cxViewController.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#import "cxEAGLView.h"
#import "cxGLViewController.h"
#import "cxAppDelegate.h"

@interface cxGLViewController ()
{

}
@end

@implementation cxGLViewController

-(cxEAGLView *)getGLView
{
    return (cxEAGLView *)self.view;
}

-(void)viewDidLoad
{
    [super viewDidLoad];
    cxAppDelegate *app = (cxAppDelegate *)[[UIApplication sharedApplication] delegate];
    cxEAGLView *glView = [[cxEAGLView alloc] initWithFrame:[app.window bounds]];
    [glView setFrame:app.window.frame];
    [glView setContentScaleFactor:app.window.contentScaleFactor];
    [glView setMultipleTouchEnabled:NO];
    self.view = glView;
    [glView release];
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskAll;
}

- (BOOL) shouldAutorotate
{
    return YES;
}

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

-(void)dealloc
{
    [super dealloc];
}

@end
