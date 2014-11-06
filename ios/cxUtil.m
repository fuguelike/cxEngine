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

void cxEngineTerminate()
{
    exit(0);
}

cxString cxUUID()
{
    uuid_t uuid;
    uuid_generate(uuid);
    return cxMD5(cxStringBinary(uuid, sizeof(uuid_t)));
}

//get strings/firstdir
cxString cxDefaultLocalized()
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
    path = [files objectAtIndex:0];
    return cxStringCreate("strings/%s",[path UTF8String]);
}

static CGSize cxCalculateStringSize(NSString *str, id font, CGSize constrainSize, NSMutableDictionary *attrs)
{
    NSArray *listItems = [str componentsSeparatedByString: @"\n"];
    CGSize dim = CGSizeZero;
    CGSize textRect = CGSizeZero;
    textRect.width = constrainSize.width > 0 ? constrainSize.width : -1;
    textRect.height = constrainSize.height > 0 ? constrainSize.height : -1;
    for (NSString *s in listItems){
        CGRect rect =  [s boundingRectWithSize:textRect options:NSStringDrawingTruncatesLastVisibleLine | NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading attributes:attrs context:nil];
        CGSize tmp = rect.size;
        if (tmp.width > dim.width){
            dim.width = tmp.width;
        }
        dim.height += tmp.height;
    }
    dim.width = ceilf(dim.width);
    dim.height = ceilf(dim.height);
    return dim;
}

#define ALIGN_TOP    1
#define ALIGN_BOTTOM 2
#define ALIGN_CENTER 3

#define cxColor4fToUIColor(c) [UIColor colorWithRed:(c).r green:(c).g blue:(c).b alpha:(c).a]

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars fontName,const cxTextAttr *attr)
{
    CX_RETURN(txt == NULL, NULL);
    NSString *str = [NSString stringWithUTF8String:txt];
    NSString *fntName = nil;
    CGSize dim = CGSizeMake(0, 0);
    CGSize csize = CGSizeMake(attr->viewSize.w, attr->viewSize.h);
    if(fontName != NULL){
        fntName = [NSString stringWithUTF8String:fontName];
        fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
    }
    UIFont *font = nil;
    if(fntName != nil){
        font = [UIFont systemFontOfSize:attr->size];
    }else{
        font = [UIFont fontWithName:fntName size:attr->size];
    }
    if(font == nil){
        font = [UIFont fontWithName:fntName size:attr->size];
    }
    NSMutableDictionary *attrs = [[NSMutableDictionary alloc] init];
    //font and color
    [attrs setObject:font forKey:NSFontAttributeName];
    [attrs setObject:cxColor4fToUIColor(attr->color) forKey:NSForegroundColorAttributeName];
    dim = cxCalculateStringSize(str, font, csize, attrs);
    // compute start point
    int startH = 0;
    int startW = 0;
    if (csize.height > dim.height){
        unsigned int vAlignment = ((int)attr->align >> 4) & 0x0F;
        if (vAlignment == ALIGN_TOP){
            startH = 0;
        }else if (vAlignment == ALIGN_CENTER){
            startH = (csize.height - dim.height) / 2;
        }else{
            startH = csize.height - dim.height;
        }
    }
    // adjust text rect
    if (csize.width > 0 && csize.width > dim.width){
        dim.width = csize.width;
    }
    if (csize.height > 0 && csize.height > dim.height){
        dim.height = csize.height;
    }
    cxInt bufsiz = sizeof(cxChar) * (int)(dim.width * dim.height * 4) + sizeof(cxSize2i);
    cxChar *buffer = allocator->malloc(bufsiz);
    memset(buffer, 0, bufsiz);
    // draw text
    CGBitmapInfo bitMapInfo = kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault;
    CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(buffer,dim.width,dim.height,8,(int)(dim.width * 4),colorSpace,bitMapInfo);
    CGContextTranslateCTM(context, 0.0f, dim.height);
    CGContextScaleCTM(context, 1.0f, -1.0f);
    UIGraphicsPushContext(context);
    cxUInt uHoriFlag = (int)attr->align & 0x0f;
    CGRect rect = CGRectMake(startW, startH, dim.width, dim.height);
    CGContextSetShouldSmoothFonts(context, true);
    CGContextSetShouldAntialias(context, true);
    CGContextSetShouldSubpixelQuantizeFonts(context, false);
    CGContextBeginTransparencyLayerWithRect(context, rect, NULL);
    //parastyle
    NSMutableParagraphStyle *parastyle = [[NSMutableParagraphStyle alloc] init];
    parastyle.alignment = (2 == uHoriFlag) ? NSTextAlignmentRight : (3 == uHoriFlag) ? NSTextAlignmentCenter : NSTextAlignmentLeft;
    parastyle.lineBreakMode = NSLineBreakByClipping;
    [attrs setObject:parastyle forKey:NSParagraphStyleAttributeName];
    [parastyle release];
    if(attr->strokeWidth > 0){
        [attrs setObject:cxColor4fToUIColor(attr->strokeColor) forKey:NSStrokeColorAttributeName];
        [attrs setObject:[NSNumber numberWithFloat:attr->strokeWidth] forKey:NSStrokeWidthAttributeName];
        [str drawInRect:rect withAttributes:attrs];
        [attrs removeObjectForKey:NSStrokeColorAttributeName];
        [attrs removeObjectForKey:NSStrokeWidthAttributeName];
    }
    [str drawInRect:rect withAttributes:attrs];
    CGContextEndTransparencyLayer(context);
    UIGraphicsPopContext();
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(context);
    [attrs release];
    //last 8bytes save widht and height
    cxSize2i *psize = (cxSize2i *)(buffer + bufsiz - sizeof(cxSize2i));
    psize->w = dim.width;
    psize->h = dim.height;
    return cxStringAttachMem(buffer, bufsiz);
}

cxBool cxAssetsExists(cxConstChars file)
{
    cxString path = cxAssetsPath(file);
    CX_RETURN(path == NULL, false);
    struct stat stat={0};
    return lstat(cxStringBody(path), &stat) == 0;
}

cxInt cxAssertsFD(cxConstChars file,cxInt *off,cxInt *length)
{
    CX_ASSERT(off != NULL && length != NULL && file != NULL, "args error");
    cxString path = cxAssetsPath(file);
    CX_ASSERT(path != NULL, "get file %s path failed",file);
    return cxFileFD(cxStringBody(path), off, length);
}

void cxEngineSendJson(cxString json)
{
    cxEngineRecvJson(json);
}

cxString cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq)
{
    cxString cxPath = cxAssetsPath(file);
    AudioStreamBasicDescription fileformat;
    UInt32 formatsize = sizeof(fileformat);
    NSString *path = [NSString stringWithUTF8String:cxStringBody(cxPath)];
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
    if(AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize) != noErr){
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
    return cxStringAttachMem(theData, (UInt32)fileDataSize);
}

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    NSLog(@"[%s:%d] %s:%s\n",file,line,type,buffer);
}

cxString cxAssetsPath(cxConstChars file)
{
    NSString *path = [[NSBundle mainBundle] resourcePath];
    if(file != NULL){
        return cxStringCreate("%s/%s",[path UTF8String],file);
    }else{
        return cxStringCreate("%s",[path UTF8String]);
    }
}

cxString cxDocumentPath(cxConstChars file)
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *path = [paths objectAtIndex:0];
    if(file != NULL){
        return cxStringCreate("%s/%s",[path UTF8String],file);
    }else{
        return cxStringConstChars([path UTF8String]);
    }
}


cxString cxLocalizedCountry()
{
    NSLocale *locale = [NSLocale currentLocale];
    return cxStringConstChars([[locale objectForKey:NSLocaleCountryCode] UTF8String]);
}

cxString cxLocalizedLang()
{
    NSLocale *locale = [NSLocale currentLocale];
    return cxStringConstChars([[locale objectForKey:NSLocaleLanguageCode] UTF8String]);
}













