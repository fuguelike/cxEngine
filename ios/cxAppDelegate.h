//
//  cxAppDelegate.h
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVAudioPlayer.h>
#import "cxGLViewController.h"
#include <engine/cxEngine.h>

@interface cxAppDelegate : UIResponder <UIApplicationDelegate,AVAudioPlayerDelegate>
{
    AVAudioPlayer *currPlayer;
    cxStr currFile;
}
@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) cxGLViewController *rootViewController;
-(void)cxPlayMusic:(cxConstChars)file volume:(cxFloat)volume loop:(cxBool)loop;
-(void)cxStopMusic;
-(void)cxPauseMusic;
-(void)cxResumeMusic;
-(void)cxSetMusicVolume:(float)volume;
@end

//
void cxSendJson(cxStr txt);

//must imp
void cxRecvJson(cxStr txt);