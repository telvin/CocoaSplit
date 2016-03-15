//
//  WindowCapture.m
//  CocoaSplit
//
//  Created by Zakk on 8/23/14.
//  Copyright (c) 2014 Zakk. All rights reserved.
//

#import "WindowCapture.h"
#import "WindowCaptureViewController.h"

@implementation WindowCapture

/* CGWindowListCreateImage sucks. It's really slow. This probably isn't a useful capture type, but whatever */


-(instancetype)init
{
    
    
    if (self = [super init])
    {
        

        _nextCaptureTime = 0.0f;
        self.captureFPS = 30.0f;
    }
    return self;
}

-(NSArray *) availableVideoDevices
{
    NSArray *windows = CFBridgingRelease(CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly|kCGWindowListExcludeDesktopElements, kCGNullWindowID));
    
    
    NSMutableArray *retArray = [[NSMutableArray alloc] init];
    
    
    NSDictionary *devinstance;
    
    for(devinstance in windows)
    {
        if (devinstance[(NSString *)kCGWindowSharingState] == kCGWindowSharingNone)
        {
            continue;
        }
        
        
        NSString *windowName = devinstance[(NSString *)kCGWindowOwnerName];
        if (!windowName)
        {
            windowName = @"?????";
        }
        
        NSNumber *windowID = devinstance[(NSString *)kCGWindowNumber];
        NSString *wUniqueID = [NSString stringWithFormat:@"%@d", windowID];
        
        
        [retArray addObject:[[CSAbstractCaptureDevice alloc] initWithName:windowName device:windowID uniqueID:wUniqueID]];
    }
    
    return retArray;
}



-(void)setActiveVideoDevice:(CSAbstractCaptureDevice *)activeVideoDevice
{
    [super setActiveVideoDevice:activeVideoDevice];
    self.captureName = activeVideoDevice.captureName;
}


-(void)frameTick
{
    
    CFAbsoluteTime currentTime = CFAbsoluteTimeGetCurrent();
    
    if (self.activeVideoDevice && currentTime >= _nextCaptureTime)
    {
        NSNumber *windowID = self.activeVideoDevice.captureDevice;
        
        
        CGImageRef windowImg = CGWindowListCreateImage(CGRectNull, kCGWindowListOptionIncludingWindow, [windowID unsignedIntValue], kCGWindowImageBoundsIgnoreFraming|kCGWindowImageBestResolution);
            [self updateLayersWithBlock:^(CALayer *layer) {
                
               layer.contents = (__bridge id)(windowImg);
            }];
            CGImageRelease(windowImg);
        
        _nextCaptureTime = currentTime + (1/self.captureFPS);
    }
}

+(NSString *)label
{
    return @"Window Capture";
}


@end
