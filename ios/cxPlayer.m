//
//  cxPlayer.m
//  cxEngineIOS
//
//  Created by xuhua on 12/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <AVFoundation/AVAudioPlayer.h>
#include <core/cxEngine.h>

static AVAudioPlayer *currPlayer = nil;

void cxPlayMusic(cxConstChars file,cxBool loop)
{
    cxString path = cxAssetsPath(file);
    CX_ASSERT(path != NULL, "path error");
    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:cxStringBody(path)]];
    //stop prev
    [currPlayer stop];
    //start new player
    NSError *error = nil;
    currPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
    CX_ASSERT(error == nil, "init audio player error %s,file=%s",[[error localizedDescription] UTF8String],file);
    if(loop){
        [currPlayer setNumberOfLoops:-1];
    }
    [currPlayer play];
    [url release];
}

void cxStopMusic()
{
    [currPlayer stop];
    currPlayer = nil;
}

void cxPauseMusic()
{
    [currPlayer pause];
}

void cxResumeMusic()
{
    [currPlayer play];
}