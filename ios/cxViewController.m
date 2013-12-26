//
//  cxViewController.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#import "cxEAGLView.h"
#import "cxViewController.h"

@interface cxViewController ()
{

}
@end

@implementation cxViewController

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

@end
