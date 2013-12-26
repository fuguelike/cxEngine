//
//  main.c
//  dirlisten
//
//  Created by xuhua on 12/4/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <CoreServices/CoreServices.h>
#include <stdio.h>

static void (fsEventStreamCallback)(ConstFSEventStreamRef streamRef,
                                    void *data,
                                    size_t numEvents,
                                    void *eventPaths,
                                    const FSEventStreamEventFlags eventFlags[],
                                    const FSEventStreamEventId eventIds[]){
    const char **paths = eventPaths;
    for(int i=0; i < numEvents; i++){
        printf("%llx = %s  %d\n",eventIds[i],paths[i],(int)numEvents);
    }
    fflush(stdout);
}
int main(int argc, const char * argv[])
{
    CFStringRef mypath = CFSTR("/Volumes/BACKUP/images");
    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&mypath, 1, NULL);
    FSEventStreamRef stream;
    CFAbsoluteTime latency = 3.0; /* Latency in seconds */
    /* Create the stream, passing in a callback */
    stream = FSEventStreamCreate(NULL,
                                 &fsEventStreamCallback,
                                 NULL,
                                 pathsToWatch,
                                 kFSEventStreamEventIdSinceNow, /* Or a previous event ID */
                                 latency,
                                 kFSEventStreamCreateFlagFileEvents /* Flags explained in reference */
                                 );
    FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(),kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);
    CFRunLoopRun();
    return 0;
}

