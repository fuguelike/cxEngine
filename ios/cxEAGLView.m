//
//  cxEAGLView.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#import "cxAppDelegate.h"
#import "cxEAGLView.h"
#import <regex.h>

@implementation cxEAGLView

+(Class)layerClass
{
    return [CAEAGLLayer class];
}

-(void)pauseMainLoop
{
    [displayLink setPaused:YES];
    cxEnginePause();
}

-(void)resumeMainLoop
{
    [displayLink setPaused:NO];
    cxEngineResume();
}

-(void)memoryWarning
{
    cxEngineMemory();
}

-(void)initMainLoop
{
    cxEngineStartup(false);
}

-(void)freeMainLoop
{
    cxEngineExit();
}

-(void)drawFrame
{
    cxEngineDraw(displayLink.duration);
    [eaglCTX presentRenderbuffer:GL_RENDERBUFFER];
}

-(void)layoutSubviews
{
    CAEAGLLayer *layer = (CAEAGLLayer*)self.layer;
    GLint cWidth,cHeight;
    
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    [eaglCTX renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &cWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &cHeight);
    CX_RETURN(cWidth == width && cHeight == height);
    
    width = cWidth;
    height = cHeight;

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        CX_ERROR("frame buff bind error");
    }
    cxEngineLayout(width, height);
}

-(void)startMainLoop
{
    cxEngine engine = cxEngineInstance();
    engine->ScaleFactor = self.contentScaleFactor;
    [displayLink setFrameInterval:engine->Interval * 60.0f];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
{
    return NO;
}

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if(self == nil){
        CX_ERROR("init gl frame error");
        return nil;
    }
    [self initMainLoop];
    CX_ASSERT(self != nil, "init view frame failed");
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    eaglLayer.opaque = YES;
    NSMutableDictionary *properties = [[NSMutableDictionary alloc] init];
    [properties setObject:[NSNumber numberWithBool:NO] forKey:kEAGLDrawablePropertyRetainedBacking];
    [properties setObject:kEAGLColorFormatRGBA8 forKey:kEAGLDrawablePropertyColorFormat];
    eaglLayer.drawableProperties = properties;
    [properties release];
    eaglCTX = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    CX_ASSERT(eaglCTX != nil,"alloc EAGL Context error");
    if(![EAGLContext setCurrentContext:eaglCTX]){
        CX_ERROR("set current eagl context error");
        return nil;
    }
    glGenFramebuffers(1, &frameBuffer);
    CX_ASSERT(frameBuffer > 0,"gl frame buffer create failed");
    glGenRenderbuffers(1, &renderBuffer);
    CX_ASSERT(renderBuffer > 0,"gl render buffer create failed");
    glGenRenderbuffers(1, &depthBuffer);
    CX_ASSERT(depthBuffer > 0,"gl depth buffer create failed");
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
    [displayLink retain];
    return self;
}

- (void) dealloc
{
    [self freeMainLoop];
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteRenderbuffers(1, &renderBuffer);
    glDeleteRenderbuffers(1, &depthBuffer);
    [EAGLContext setCurrentContext:nil];
    frameBuffer = 0;
    renderBuffer = 0;
    depthBuffer = 0;
    [eaglCTX release];
    [displayLink release];
    [super dealloc];
}

-(void)dispatcherMotionEvent:(NSSet *)touches type:(cxTouchType)type event:(UIEvent *)event
{
    cxTouchPoint points[CX_MAX_TOUCH_POINT];
    cxInt num = 0;
    for (UITouch *touch in touches) {
        CGPoint point = [touch locationInView:[touch view]];
        cxVec2f pos = cxVec2fv(point.x * self.contentScaleFactor, point.y * self.contentScaleFactor);
        points[num].xy = pos;
        points[num].id = (cxLong)touch;
        num ++;
    }
    cxEngineFireTouch(type, num, points);
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self dispatcherMotionEvent:touches type:cxTouchTypeDown event:event];
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self dispatcherMotionEvent:touches type:cxTouchTypeMove event:event];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self dispatcherMotionEvent:touches type:cxTouchTypeUp event:event];
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self dispatcherMotionEvent:touches type:cxTouchTypeCancel event:event];
}
@end
