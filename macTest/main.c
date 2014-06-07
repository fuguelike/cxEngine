//
//  main.c
//  macTest
//
//  Created by xuhua on 6/7/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <cxcore/cxBase.h>

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    printf("[%s:%d] %s:%s\n",file,line,type,buffer);
    fflush(stdout);
}

int main(int argc, const char * argv[])
{
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

