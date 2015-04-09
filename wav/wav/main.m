//
//  main.m
//  wav
//
//  Created by xuhua on 2/6/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreAudio/CoreAudio.h>
#import <AudioToolbox/AudioFile.h>

#define P(n,f)  printf(#n":"f"\n",fileformat.m##n)

void *cxWAVSamplesWithFile(const char*file,int *format,int *freq,int *siz)
{
    AudioStreamBasicDescription fileformat;
    UInt32 formatsize = sizeof(fileformat);
    NSString *path = [NSString stringWithUTF8String:file];
    CFURLRef fileURL = (__bridge CFURLRef)[NSURL fileURLWithPath:path];
    UInt64 fileDataSize = 0;
    UInt32 thePropertySize = sizeof(UInt64);
    AudioFileID afid = NULL;
    void *theData = NULL;
    if(AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, &afid) != noErr){
        goto completed;
    }
    if(AudioFileGetProperty(afid, kAudioFilePropertyDataFormat, &formatsize, &fileformat) != noErr){
        goto completed;
    }
    OSStatus status = AudioFileGetProperty(afid, kAudioFilePropertyAudioDataByteCount, &thePropertySize, &fileDataSize);
    if(status == noErr){
        *siz = (int)fileDataSize;
        theData = malloc(fileDataSize);
        AudioFileReadBytes(afid, false, 0, (UInt32 *)&fileDataSize, theData);
    }
completed:
    printf("-------wav description---------\n");
    P(SampleRate,"%f");
    P(FormatID,"%u");
    P(FormatFlags,"%u");
    P(BytesPerPacket,"%u");
    P(FramesPerPacket,"%u");
    P(BytesPerFrame,"%u");
    P(ChannelsPerFrame,"%u");
    P(BitsPerChannel,"%u");
    P(Reserved,"%u");
    printf("-------------------------------\n");
    AudioFileClose(afid);
    return theData;
}

int main(int argc, const char * argv[]) {
    if(argc != 3){
        printf("use:wav 1.wav 1.pcm\n");
        return 2;
    }
    int fmt,freq,siz;
    void *data = cxWAVSamplesWithFile(argv[1], &fmt, &freq, &siz);
    if(data == NULL){
        return 1;
    }
    FILE *fd = fopen(argv[2], "wb");
    fwrite(data, siz, 1, fd);
    fclose(fd);
    return 0;
}
