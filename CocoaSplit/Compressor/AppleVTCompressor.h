//
//  AppleVTCompressor.h
//  streamOutput
//
//  Created by Zakk on 3/17/13.
//  Copyright (c) 2013 Zakk. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CaptureController.h"
#import "CompressorBase.h"

#import <VideoToolbox/VideoToolbox.h>
#import <VideoToolbox/VTVideoEncoderList.h>

@interface AppleVTCompressor : CompressorBase <h264Compressor, NSCoding>
{
    
    VTCompressionSessionRef _compression_session;
    VTPixelTransferSessionRef _vtpt_ref;

}


//@property (strong) id<ControllerProtocol> settingsController;
//@property (strong) id<ControllerProtocol> outputDelegate;

@property (assign) int average_bitrate;
@property (assign) int max_bitrate;
@property (assign) int keyframe_interval;
@property (strong) NSString *profile;
@property (assign) BOOL use_cbr;
@property (strong) NSArray *profiles;


-(bool)compressFrame:(CapturedFrameData *)frameData;


@end
 