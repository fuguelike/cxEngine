//
//  cxAppDelegate.h
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVAudioPlayer.h>
#import "cxViewController.h"
#include <core/cxEngine.h>

@interface cxAppDelegate : UIResponder <UIApplicationDelegate,AVAudioPlayerDelegate>
{
    AVAudioPlayer *currPlayer;
    cxString currFile;
}
@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) cxViewController *viewController;
-(void)cxPlayMusic:(cxConstChars)file loop:(cxBool)loop;
-(void)cxStopMusic;
-(void)cxPauseMusic;
-(void)cxResumeMusic;
@end
