//
//  cxEAGLView.h
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface cxEAGLView : UIView
{
    GLuint  frameBuffer;
    GLuint  colorBuffer;
    GLuint  depthBuffer;
    GLint   width;
    GLint   height;
    EAGLContext *eaglCTX;
    CADisplayLink *displayLink;
}

-(void)pauseMainLoop;

-(void)resumeMainLoop;

-(void)memoryWarning;

-(void)startMainLoop;

-(void)initMainLoop;

-(void)freeMainLoop;

+(cxEAGLView *)glView;

-(id)initWithFrame:(CGRect)frame;
@end

extern cxEAGLView *instance;
