//
//  h264Compressor.h
//  streamOutput
//
//  Created by Zakk on 3/17/13.
//  Copyright (c) 2013 Zakk. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import "CapturedFrameData.h"

//#import "OutputDestination.h"


@class CaptureController;
@class OutputDestination;
@protocol ControllerProtocol;


@protocol h264Compressor <NSObject,NSCoding>

//compressFrame is expected to be non-blocking. Create a serial dispatch queue if the underlying compressor
//is blocking

-(bool)compressFrame:(CapturedFrameData *)imageBuffer;


-(bool)setupCompressor:(CVPixelBufferRef)videoFrame;




@property (strong) CaptureController *settingsController;
@property (assign) bool isNew;
@property (strong) NSMutableString *name;
@property (strong) NSString *compressorType;
@property (assign) int width;
@property (assign) int height;
@property (strong) NSString *resolutionOption;
@property (assign) bool errored;


-(void) addOutput:(id)destination;
-(void) removeOutput:(id)destination;
-(bool) hasOutputs;
-(void) reset;
-(bool) validate:(NSError **)therror;
-(void) addAudioData:(CMSampleBufferRef)audioData;







@end
