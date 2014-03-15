
#import <UIKit/UIKit.h>
#import "cxAppDelegate.h"
#include <core/cxGlobal.h>

int main(int argc, char * argv[])
{
    cxGlobalInit();
    int r = 0;
    @autoreleasepool {
        r = UIApplicationMain(argc, argv, nil, NSStringFromClass([cxAppDelegate class]));
    }
    cxGlobalFree();
    return r;
}