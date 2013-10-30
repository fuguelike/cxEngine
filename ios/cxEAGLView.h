//
//  cxEAGLView.h
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include <core/cxEngine.h>

@interface cxEAGLView : UIView {
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

-(id)initWithFrame:(CGRect)frame;
@end
