//
//  cxPlayer.m
//  cxEngineIOS
//
//  Created by xuhua on 12/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import "cxAppDelegate.h"
#include <engine/cxEngine.h>

void cxPlayMusic(cxConstChars file,cxBool loop)
{
    cxAppDelegate *app = (cxAppDelegate *)[[UIApplication sharedApplication] delegate];
    [app cxPlayMusic:file loop:loop];
}

void cxStopMusic()
{
    cxAppDelegate *app = (cxAppDelegate *)[[UIApplication sharedApplication] delegate];
    [app cxStopMusic];
}

void cxPauseMusic()
{
    cxAppDelegate *app = (cxAppDelegate *)[[UIApplication sharedApplication] delegate];
    [app cxPauseMusic];
}

void cxResumeMusic()
{
    cxAppDelegate *app = (cxAppDelegate *)[[UIApplication sharedApplication] delegate];
    [app cxResumeMusic];
}