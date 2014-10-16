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
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>
#include <engine/cxEngine.h>

void cxEngineTerminate()
{
    exit(0);
}

cxArray cxFontNames()
{
    cxArray list = CX_CREATE(cxArray);
    NSArray* familys = [UIFont familyNames];
    for (NSString *family  in familys) {
        NSArray* fonts = [UIFont fontNamesForFamilyName:family];
        for (NSString *font in fonts) {
            cxString item = cxStringAllocChars([font UTF8String]);
            cxArrayAppend(list, item);
            CX_RELEASE(item);
        }
    }
    return list;
}

static CGSize cxCalculateStringSize(NSString *str, id font, CGSize *constrainSize, NSMutableDictionary *attrs)
{
    NSArray *listItems = [str componentsSeparatedByString: @"\n"];
    CGSize dim = CGSizeZero;
    CGSize textRect = CGSizeZero;
    textRect.width = constrainSize->width > 0 ? constrainSize->width : -1;
    textRect.height = constrainSize->height > 0 ? constrainSize->height : -1;
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
#define ALIGN_CENTER 3
#define ALIGN_BOTTOM 2

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars fontName,cxFloat size,cxTextAlign align,cxInt cw,cxInt ch)
{
    CX_RETURN(txt == NULL, NULL);
    NSString *str = [NSString stringWithUTF8String:txt];
    NSString *fntName = nil;
    CGSize dim, constrainSize;
    constrainSize.width = cw;
    constrainSize.height = ch;
    if(fontName != NULL){
        fntName = [NSString stringWithUTF8String:fontName];
        fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
    }
    UIFont *font = nil;
    if(fntName != nil){
        font = [UIFont systemFontOfSize:size];
    }else{
        font = [UIFont fontWithName:fntName size:size];
    }
    if(font == nil){
        font = [UIFont fontWithName:fntName size:size];
    }
    NSMutableDictionary *attrs = [[NSMutableDictionary alloc] init];
    [attrs setObject:font forKey:NSFontAttributeName];
    
    dim = cxCalculateStringSize(str, font, &constrainSize, attrs);
    // compute start point
    int startH = 0;
    int startW = 2;
    dim.width += 2;
    if (constrainSize.height > dim.height){
        // vertical alignment
        unsigned int vAlignment = ((int)align >> 4) & 0x0F;
        if (vAlignment == ALIGN_TOP){
            startH = 0;
        }else if (vAlignment == ALIGN_CENTER){
            startH = (constrainSize.height - dim.height) / 2;
        }else{
            startH = constrainSize.height - dim.height;
        }
    }
    // adjust text rect
    if (constrainSize.width > 0 && constrainSize.width > dim.width){
        dim.width = constrainSize.width;
    }
    if (constrainSize.height > 0 && constrainSize.height > dim.height){
        dim.height = constrainSize.height;
    }
    cxInt bufsiz = sizeof(cxChar) * (int)(dim.width * dim.height * 4) + sizeof(cxSize2i);
    cxChar *buffer = allocator->malloc(bufsiz);
    memset(buffer, 0, bufsiz);
    // draw text
    CGBitmapInfo bitMapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
    CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(buffer,dim.width,dim.height,8,(int)(dim.width) * 4,colorSpace,bitMapInfo);
    CGContextSetRGBFillColor(context, 1.0f, 1.0f, 1.0f, 1.0f);
    CGContextTranslateCTM(context, 0.0f, dim.height);
    CGContextScaleCTM(context, 1.0f, -1.0f);
    
    UIGraphicsPushContext(context);
    cxUInt uHoriFlag = (int)align & 0x0f;
    NSTextAlignment nsAlign = (2 == uHoriFlag) ? NSTextAlignmentRight : (3 == uHoriFlag) ? NSTextAlignmentCenter : NSTextAlignmentLeft;
    
    CGRect rect = CGRectMake(startW, startH, dim.width, dim.height);
    CGContextSetShouldSubpixelQuantizeFonts(context, false);
    CGContextBeginTransparencyLayerWithRect(context, rect, NULL);
    
    NSMutableParagraphStyle *parastyle = [[NSMutableParagraphStyle alloc] init];
    parastyle.alignment = nsAlign;
    parastyle.lineBreakMode = NSLineBreakByCharWrapping;
    [attrs setObject:parastyle forKey:NSParagraphStyleAttributeName];
    
    [str drawInRect:rect withAttributes:attrs];
    
    [parastyle release];
    
    CGContextEndTransparencyLayer(context);
    UIGraphicsPopContext();
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(context);
    [attrs release];
    //last 8bytes save widht and height
    cxAny psize = buffer + bufsiz - sizeof(cxSize2i);
    ((cxSize2i *)psize)->w = dim.width;
    ((cxSize2i *)psize)->h = dim.height;
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

cxBool cxWriteFile(cxString file,cxString data)
{
    FILE *fd = fopen(cxStringBody(file), "wb");
    CX_RETURN(fd == NULL, -1);
    cxInt bytes = (cxInt)fwrite(cxStringBody(data), cxStringLength(data), 1, fd);
    fclose(fd);
    return bytes > 0;
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
    if(AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, &afid)){
        CX_ERROR("open url %s error",file);
        goto completed;
    }
    if(AudioFileGetProperty(afid, kAudioFilePropertyDataFormat, &formatsize, &fileformat)){
        CX_ERROR("get format error");
        goto completed;
    }
    if(!AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize)){
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
    if(theData == NULL){
        return NULL;
    }
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

cxString cxLocalizedLang()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
    NSString *current = [languages objectAtIndex:0];
    NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:current];
    NSString * lngcode = [temp objectForKey:NSLocaleLanguageCode];
    CX_LOGGER("current localized lang:%s",[lngcode UTF8String]);
    return cxStringCreate("%s",[lngcode UTF8String]);
}
