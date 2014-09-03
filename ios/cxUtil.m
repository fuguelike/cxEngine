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

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars fontName,cxTextAlign align,cxFloat size, cxTextureTXTConfig *info)
{
    CX_ASSERT(info != NULL, "args error");
    CX_RETURN(txt == NULL, NULL);
    cxString rv = NULL;
    NSString * str = [NSString stringWithUTF8String:txt];
    NSString * fntName = nil;
    CGSize dim, constrainSize;
    constrainSize.width = info->size.w;
    constrainSize.height = info->size.h;
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
        return NULL;
    }
    NSMutableDictionary *attrs = [[NSMutableDictionary alloc] init];
    [attrs setObject:font forKey:NSFontAttributeName];
    dim = cxCalculateStringSize(str, font, &constrainSize, attrs);
    // compute start point
    int startH = 0;
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
    cxFloat shadowStrokePaddingX = 0.0f;
    cxFloat shadowStrokePaddingY = 0.0f;
    if ( info->hasStroke ){
        shadowStrokePaddingX = ceilf(info->strokeSize);
        shadowStrokePaddingY = ceilf(info->strokeSize);
    }
    // add the padding (this could be 0 if no shadow and no stroke)
    dim.width  += shadowStrokePaddingX*2;
    dim.height += shadowStrokePaddingY*2;
    cxInt bufsiz = sizeof(cxChar) * (int)(dim.width * dim.height * 4) + sizeof(cxInt) * 2;
    cxChar *buffer = allocator->malloc(bufsiz);
    memset(buffer, 0, bufsiz);
    // draw text
    CGBitmapInfo bitMapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
    CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(buffer,dim.width,dim.height,8,(int)(dim.width) * 4,colorSpace,bitMapInfo);
    if (context == NULL){
        CGColorSpaceRelease(colorSpace);
        goto completed;
    }
    CGContextSetRGBFillColor(context, info->tintColor.r, info->tintColor.g, info->tintColor.b, 1);
    CGContextTranslateCTM(context, 0.0f, (dim.height - shadowStrokePaddingY) );
    CGContextScaleCTM(context, 1.0f, -1.0f);
    UIGraphicsPushContext(context);
    unsigned uHoriFlag = (int)align & 0x0f;
    NSTextAlignment nsAlign = (2 == uHoriFlag) ? NSTextAlignmentRight : (3 == uHoriFlag) ? NSTextAlignmentCenter : NSTextAlignmentLeft;
    CGColorSpaceRelease(colorSpace);
    cxFloat textOriginX  = 0;
    cxFloat textOrigingY = startH;
    cxFloat textWidth    = dim.width;
    cxFloat textHeight   = dim.height;
    CGRect rect = CGRectMake(textOriginX, textOrigingY, textWidth, textHeight);
    CGContextSetShouldSubpixelQuantizeFonts(context, false);
    CGContextBeginTransparencyLayerWithRect(context, rect, NULL);
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    paragraphStyle.alignment = nsAlign;
    paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
    [attrs setObject:paragraphStyle forKey:NSParagraphStyleAttributeName];
    UIColor *tintColor = [UIColor colorWithRed:info->tintColor.r green:info->tintColor.g blue:info->tintColor.b alpha:1.0f];
    [attrs setObject:tintColor forKey:NSForegroundColorAttributeName];
    if (info->hasStroke){
        CGContextSetTextDrawingMode(context, kCGTextStroke);
        [attrs setObject:[NSNumber numberWithFloat: info->strokeSize / size * 100 ] forKey:NSStrokeWidthAttributeName];
        UIColor *strokeColor = [UIColor colorWithRed:info->strokeColor.r green:info->strokeColor.g blue:info->strokeColor.b alpha:1.0f];
        [attrs setObject:strokeColor forKey:NSStrokeColorAttributeName];
        [str drawInRect:rect withAttributes:attrs];
    }
    [paragraphStyle release];
    CGContextSetTextDrawingMode(context, kCGTextFill);
    [attrs setObject:font forKey:NSFontAttributeName];
    [str drawInRect:rect withAttributes:attrs];
    CGContextEndTransparencyLayer(context);
    UIGraphicsPopContext();
    CGContextRelease(context);
    [attrs release];
    ((cxSize2i *)buffer)->w = dim.width;
    ((cxSize2i *)buffer)->h = dim.height;
    rv = cxStringAttach(buffer, bufsiz);
completed:
    return rv;
}

cxBool cxAssetsExists(cxConstChars file)
{
    cxString path = cxAssetsPath(file);
    CX_RETURN(path == NULL, false);
    struct stat stat={0};
    return lstat(cxStringBody(path), &stat) == 0;
}

cxBool cxWriteFile(cxString file,cxString data)
{
    FILE *fd = fopen(cxStringBody(file), "wb");
    CX_RETURN(fd == NULL, -1);
    cxInt bytes = fwrite(cxStringBody(data), cxStringLength(data), 1, fd);
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
    cxPointer theData = NULL;
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
    return cxStringAttach(theData, (UInt32)fileDataSize);
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
