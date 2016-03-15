//
//  CSDeckLinkCapture.h
//  CSDeckLinkCapturePlugin
//
//  Created by Zakk on 6/13/15.
//  Copyright (c) 2015 Zakk. All rights reserved.
//

#import "CSCaptureBase.h"
#import "DeckLinkBridge.h"
#import "CSDeckLinkDevice.h"
#import "CSDeckLinkLayer.h"
#import "CSDeckLinkWrapper.h"


@interface CSDeckLinkCapture : CSCaptureBase <CSCaptureSourceProtocol>
{
    DeckLinkDeviceDiscovery *_discoveryDev;
    NSString *_restoredMode;
    NSString *_restoredFormat;
    NSString *_restoredInput;
}

@property (strong) CSDeckLinkDevice *currentInput;
@property (assign) frame_render_behavior renderType;


-(void)addDevice:(CSAbstractCaptureDevice *)device;

-(void)frameArrived:(IDeckLinkVideoFrame *)frame;
-(void)removeDevice:(IDeckLink *)device;




@end
