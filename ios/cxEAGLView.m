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

cxEAGLView *instance = nil;

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
    cxEngineBegin();
}

-(void)freeMainLoop
{
    cxEngineDestroy();
}

-(void)drawFrame
{
    cxEngineDraw();
    [eaglCTX presentRenderbuffer:GL_RENDERBUFFER];
}

-(void)layoutSubviews
{
    [self resizeFromLayer:(CAEAGLLayer*)self.layer];
    cxEngineLayout(width, height);
}

-(void)startMainLoop
{
    cxEngine engine = cxEngineInstance();
    [displayLink setFrameInterval:engine->interval * 60.0f];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    instance = self;
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
    }
    glGenFramebuffers(1, &frameBuffer);
    CX_ASSERT(frameBuffer > 0,"gl frame buffer create failed");
    glGenRenderbuffers(1, &colorBuffer);
    CX_ASSERT(colorBuffer > 0,"gl color buffer create failed");
    glGenRenderbuffers(1, &depthBuffer);
    CX_ASSERT(depthBuffer > 0,"gl depth buffer create failed");
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);
    [self initMainLoop];
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
    [displayLink retain];
    
    return self;
}

+(cxEAGLView *)glView
{
    cxAppDelegate *app = (cxAppDelegate *)[[UIApplication sharedApplication] delegate];
    return (cxEAGLView *)app.viewController.view;
}

-(void)resizeFromLayer:(CAEAGLLayer *)layer
{
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
    [eaglCTX renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
}

- (void) dealloc
{
    [self freeMainLoop];
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteRenderbuffers(1, &colorBuffer);
    glDeleteRenderbuffers(1, &depthBuffer);
    [EAGLContext setCurrentContext:nil];
    frameBuffer = 0;
    colorBuffer = 0;
    depthBuffer = 0;
    [eaglCTX release];
    [displayLink release];
    [super dealloc];
}

-(void)dispatcherMotionEvent:(NSSet *)touches type:(cxTouchType)type event:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint point = [touch locationInView:[touch view]];
    cxVec2f pos = cxVec2fv(point.x * self.contentScaleFactor, point.y * self.contentScaleFactor);
    cxEngineFireTouch(type, pos);
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
