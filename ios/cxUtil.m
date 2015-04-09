//
//  cxUtil.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <uuid/uuid.h>
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>
#include <engine/cxEngine.h>
#import <ios/cxAppDelegate.h>
#import <CoreText/CoreText.h>

void cxEngineTerminate()
{
    cxEngineExit();
    exit(0);
}

cxStr cxUUID()
{
    uuid_t uuid;
    uuid_generate(uuid);
    return cxMD5(cxStrBinary(uuid, sizeof(uuid_t)));
}

//get strings/firstdir
cxStr cxDefaultLocalized(cxConstChars country,cxConstChars lang)
{
    NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/strings"];
    if(path == nil){
        return NULL;
    }
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *error = nil;
    NSArray *files = [fm contentsOfDirectoryAtPath:path error:&error];
    if(error != nil || files.count == 0){
        return NULL;
    }
    NSString *slang = [NSString stringWithUTF8String:lang];
    for (NSString *dir in files) {
        if(![dir hasPrefix:slang]){
            continue;
        }
        return cxStrCreate("strings/%s",[dir UTF8String]);
    }
    path = [files objectAtIndex:0];
    return cxStrCreate("strings/%s",[path UTF8String]);
}

#define cxColor4fToUIColor(c) [UIColor colorWithRed:(c).r green:(c).g blue:(c).b alpha:(c).a]

CTFrameRef frameRefFromSize(NSString *str, CGRect rect,UIFont *font,cxTextAlign align,UIColor *color)
{
    // Set up font.
    CTFontRef fontRef = (__bridge CTFontRef)font;
    NSTextAlignment textAlign = NSTextAlignmentLeft;
    if(align == cxTextAlignCenter){
        textAlign = NSTextAlignmentCenter;
    }else if(align == cxTextAlignLeft){
        textAlign = NSTextAlignmentLeft;
    }else{
        textAlign = NSTextAlignmentRight;
    }
    CTTextAlignment alignment = NSTextAlignmentToCTTextAlignment(textAlign);
    CTLineBreakMode lineBreakMode = kCTLineBreakByClipping;
    CTParagraphStyleSetting paragraphStyleSettings[] = {
        {kCTParagraphStyleSpecifierAlignment, sizeof(CTTextAlignment), &alignment},
        {kCTParagraphStyleSpecifierLineBreakMode, sizeof(CTLineBreakMode), &lineBreakMode}
    };
    CTParagraphStyleRef paragraphStyleRef = CTParagraphStyleCreate(paragraphStyleSettings, 2);
    
    // Set up attributed string.
    CFStringRef keys[] = {kCTFontAttributeName, kCTParagraphStyleAttributeName, kCTForegroundColorAttributeName};
    CFTypeRef values[] = {fontRef, paragraphStyleRef, color.CGColor};
    CFDictionaryRef attributes = CFDictionaryCreate(kCFAllocatorDefault, (const void **)&keys, (const void **)&values, sizeof(keys) / sizeof(keys[0]), &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    CFRelease(paragraphStyleRef);
    
    CFStringRef stringRef = (__bridge CFStringRef)str;
    CFAttributedStringRef attributedStringRef = CFAttributedStringCreate(kCFAllocatorDefault, stringRef, attributes);
    CFRelease(attributes);
    
    // Set up frame.
    CTFramesetterRef framesetterRef = CTFramesetterCreateWithAttributedString(attributedStringRef);
    CFRelease(attributedStringRef);

    CGMutablePathRef pathRef = CGPathCreateMutable();
    CGPathAddRect(pathRef, NULL, rect);
    
    CTFrameRef frameRef = CTFramesetterCreateFrame(framesetterRef, CFRangeMake(0, str.length), pathRef, NULL);
    CFRelease(framesetterRef);
    CGPathRelease(pathRef);
    return frameRef;
}

CGImageRef strokeImageWithRect(CGRect rect,CTFrameRef frameRef,CGFloat strokeSize,UIColor *strokeColor)
{
    // Create context.
    UIGraphicsBeginImageContextWithOptions(rect.size, NO, 0.0);
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetTextDrawingMode(context, kCGTextStroke);
    
    // Draw clipping mask.
    CGContextSetLineWidth(context, strokeSize);
    CGContextSetLineJoin(context, kCGLineJoinRound);
    [[UIColor whiteColor] setStroke];
    CTFrameDraw(frameRef, context);
    
    // Save clipping mask.
    CGImageRef clippingMask = CGBitmapContextCreateImage(context);
    
    // Clear the content.
    CGContextClearRect(context, rect);
    
    // Draw stroke.
    CGContextClipToMask(context, rect, clippingMask);
    CGContextTranslateCTM(context, 0.0, CGRectGetHeight(rect));
    CGContextScaleCTM(context, 1.0, -1.0);
    [strokeColor setFill];
    UIRectFill(rect);
    
    // Clean up and return image.
    CGImageRelease(clippingMask);
    CGImageRef image = CGBitmapContextCreateImage(context);
    UIGraphicsEndImageContext();
    return image;
}

cxStr cxCreateTXTTextureData(cxConstChars txt,cxConstChars fontName,const cxTextAttr *attr)
{
    CX_RETURN(txt == NULL, NULL);
    NSString *str = [NSString stringWithUTF8String:txt];
    NSString *fntName = nil;
    if(fontName != NULL){
        fntName = [NSString stringWithUTF8String:fontName];
        fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
    }
    UIFont *font = nil;
    if(fntName != nil){
        if(attr->boldFont)fntName = [fntName stringByAppendingString:@"-Bold"];
        font = [UIFont fontWithName:fntName size:attr->size];
    }
    if(font == nil){
        if(attr->boldFont)font = [UIFont boldSystemFontOfSize:attr->size];
        else font = [UIFont systemFontOfSize:attr->size];
    }
    NSMutableDictionary *attrs = [[NSMutableDictionary alloc] init];
    [attrs setObject:font forKey:NSFontAttributeName];
    CGSize dim = [str sizeWithAttributes:attrs];
    [attrs release];
    dim.width  = (int)ceilf(dim.width) + attr->strokeWidth;
    dim.height = (int)ceilf(dim.height)+ attr->strokeWidth;
    cxInt bufsiz = sizeof(cxChar) * (int)(dim.width * dim.height * 4) + sizeof(cxSize2i);
    cxChar *buffer = allocator->malloc(bufsiz);
    CGBitmapInfo bitMapInfo = kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault;
    CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(buffer,dim.width,dim.height,8,(int)(dim.width * 4),colorSpace,bitMapInfo);
    CGColorSpaceRelease(colorSpace);
    CX_ASSERT(context != NULL, "CGBitmapContextCreate create error");
    CGRect rect = CGRectMake(0, 0, dim.width, dim.height);
    CGContextTranslateCTM(context, 0.0f, dim.height);
    CGContextScaleCTM(context, 1.0f, -1.0f);
    UIGraphicsPushContext(context);
    UIGraphicsBeginImageContextWithOptions(rect.size, NO, 0.0);
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CTFrameRef frameRef = frameRefFromSize(str, rect, font, attr->align, cxColor4fToUIColor(attr->color));
    CGContextTranslateCTM(ctx, 0.0, CGRectGetHeight(rect));
    CGContextScaleCTM(ctx, 1.0, -1.0);
    if([str length] > 0){
        CGContextSaveGState(ctx);
        CGContextSetTextDrawingMode(ctx, kCGTextFill);
        CTFrameDraw(frameRef, ctx);
        CGContextRestoreGState(ctx);
    }
    if(attr->strokeWidth > 0){
        CGRect srect = rect;
        srect.origin.x += attr->strokeOffset.x;
        srect.origin.y += attr->strokeOffset.y;
        CGContextSaveGState(ctx);
        CGContextSetTextDrawingMode(ctx, kCGTextStroke);
        CGImageRef image = CGBitmapContextCreateImage(ctx);
        CGImageRef strokeImage = strokeImageWithRect(srect,frameRef,attr->strokeWidth * 2,cxColor4fToUIColor(attr->strokeColor));
        CGContextDrawImage(ctx, srect, strokeImage);
        CGContextDrawImage(ctx, srect, image);
        CGImageRelease(strokeImage);
        CGImageRelease(image);
        CGContextRestoreGState(ctx);
    }
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    [image drawInRect:rect];
    CFRelease(frameRef);
    UIGraphicsPopContext();
    CGContextRelease(context);
    cxSize2i *psize = (cxSize2i *)(buffer + bufsiz - sizeof(cxSize2i));
    psize->w = dim.width;
    psize->h = dim.height;
    return cxStrAttachMem(buffer, bufsiz);
}

cxBool cxAssetsExists(cxConstChars file)
{
    cxStr path = cxAssetsPath(file);
    CX_RETURN(path == NULL, false);
    struct stat stat={0};
    return lstat(cxStrBody(path), &stat) == 0;
}

cxInt cxAssertsFD(cxConstChars file,cxInt *off,cxInt *length)
{
    CX_ASSERT(off != NULL && length != NULL && file != NULL, "args error");
    cxStr path = cxAssetsPath(file);
    CX_ASSERT(path != NULL, "get file %s path failed",file);
    return cxFileFD(cxStrBody(path), off, length);
}

void cxSendJson(cxStr txt)
{
     cxEngineRecvJson(txt);
}

//ios 收到gl json请求
void cxEngineSendJson(cxStr txt)
{
    cxRecvJson(txt);
}

cxStr cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq)
{
    cxStr cxPath = cxAssetsPath(file);
    AudioStreamBasicDescription fileformat;
    UInt32 formatsize = sizeof(fileformat);
    NSString *path = [NSString stringWithUTF8String:cxStrBody(cxPath)];
    CFURLRef fileURL = (__bridge CFURLRef)[NSURL fileURLWithPath:path];
    UInt64 fileDataSize = 0;
    UInt32 thePropertySize = sizeof(UInt64);
    AudioFileID afid = NULL;
    cxAny theData = NULL;
    if(AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, &afid) != noErr){
        CX_ERROR("open url %s error",file);
        goto completed;
    }
    if(AudioFileGetProperty(afid, kAudioFilePropertyDataFormat, &formatsize, &fileformat) != noErr){
        CX_ERROR("get format error");
        goto completed;
    }
    if(AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize) == noErr){
        theData = allocator->malloc((cxInt)fileDataSize);
        AudioFileReadBytes(afid, false, 0, (UInt32 *)&fileDataSize, theData);
        *freq = (ALsizei)fileformat.mSampleRate;
        if(fileformat.mBitsPerChannel == 16){
            *format = (fileformat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
        }else{
            *format = (fileformat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
        }
    }
completed:
    AudioFileClose(afid);
    CX_RETURN(theData == NULL,NULL);
    return cxStrAttachMem(theData, (UInt32)fileDataSize);
}

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    NSLog(@"[%s:%d] %s:%s\n",file,line,type,buffer);
}

cxStr cxAssetsPath(cxConstChars file)
{
    NSString *path = [[NSBundle mainBundle] resourcePath];
    if(file != NULL){
        return cxStrCreate("%s/%s",[path UTF8String],file);
    }else{
        return cxStrCreate("%s",[path UTF8String]);
    }
}

cxStr cxDocumentPath(cxConstChars file)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *path = [paths objectAtIndex:0];
    if(file != NULL){
        return cxStrCreate("%s/%s",[path UTF8String],file);
    }else{
        return cxStrConstChars([path UTF8String]);
    }
}

cxStr cxLocalizedCountry()
{
    NSLocale *locale = [NSLocale currentLocale];
    return cxStrConstChars([[locale objectForKey:NSLocaleCountryCode] UTF8String]);
}

cxStr cxLocalizedLang()
{
    NSLocale *locale = [NSLocale currentLocale];
    return cxStrConstChars([[locale objectForKey:NSLocaleLanguageCode] UTF8String]);
}













