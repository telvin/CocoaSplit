//
//  AppleVTCompressor.m
//  streamOutput
//
//  Created by Zakk on 3/17/13.
//  Copyright (c) 2013 Zakk. All rights reserved.
//

#import "AppleVTCompressor.h"
#import "OutputDestination.h"

OSStatus VTCompressionSessionCopySupportedPropertyDictionary(VTCompressionSessionRef, CFDictionaryRef *);



@implementation AppleVTCompressor


- (id)copyWithZone:(NSZone *)zone
{
    AppleVTCompressor *copy = [[[self class] allocWithZone:zone] init];
    
    copy.settingsController = self.settingsController;
    
    copy.isNew = self.isNew;
    
    copy.name = self.name;
    copy.average_bitrate = self.average_bitrate;
    copy.max_bitrate = self.max_bitrate;
    
    copy.compressorType = self.compressorType;
    
    copy.profile = self.profile;
    copy.keyframe_interval = self.keyframe_interval;
    copy.use_cbr = self.use_cbr;
    
    copy.width = self.width;
    copy.height = self.height;
    copy.working_width = self.width;
    copy.working_height = self.height;
    
    copy.resolutionOption = self.resolutionOption;

    return copy;
}


-(void) encodeWithCoder:(NSCoder *)aCoder
{
    
    
    [aCoder encodeObject:self.name forKey:@"name"];
    [aCoder encodeInteger:self.average_bitrate forKey:@"average_bitrate"];
    [aCoder encodeInteger:self.max_bitrate forKey:@"max_bitrate"];
    [aCoder encodeInteger:self.keyframe_interval forKey:@"keyframe_interval"];
    [aCoder encodeObject:self.profile forKey:@"profile"];
    [aCoder encodeBool:self.use_cbr forKey:@"use_cbr"];
    
    [aCoder encodeInteger:self.width forKey:@"videoWidth"];
    [aCoder encodeInteger:self.height forKey:@"videoHeight"];

    [aCoder encodeObject:self.resolutionOption forKey:@"resolutionOption"];

}

-(id) initWithCoder:(NSCoder *)aDecoder
{
    if (self = [self init])
    {
        self.name = [aDecoder decodeObjectForKey:@"name"];
        self.average_bitrate = (int)[aDecoder decodeIntegerForKey:@"average_bitrate"];
        self.max_bitrate = (int)[aDecoder decodeIntegerForKey:@"max_bitrate"];
        self.keyframe_interval = (int)[aDecoder decodeIntegerForKey:@"keyframe_interval"];
        self.profile = [aDecoder decodeObjectForKey:@"profile"];
        self.use_cbr = [aDecoder decodeBoolForKey:@"use_cbr"];
        self.width = (int)[aDecoder decodeIntegerForKey:@"videoWidth"];
        self.height = (int)[aDecoder decodeIntegerForKey:@"videoHeight"];
        if ([aDecoder containsValueForKey:@"resolutionOption"])
        {
            self.resolutionOption = [aDecoder decodeObjectForKey:@"resolutionOption"];
        }

        
    }
    
    return self;
}


-(id)init
{
    if (self = [super init])
    {
        

        
        self.compressorType = @"AppleVTCompressor";

        self.profiles = @[[NSNull null], @"Baseline", @"Main", @"High"];
    }
    
    return self;
}


-(void) reset
{

    
    self.errored = NO;
    VTCompressionSessionInvalidate(_compression_session);
    if (_compression_session)
    {
        CFRelease(_compression_session);
    }
    
    _compression_session = nil;

}


- (void) dealloc
{
    [self reset];
}



-(NSString *)description
{
    return [NSString stringWithFormat:@"%@: Type: %@, Average Bitrate %d, Max Bitrate %d, CBR: %d, Profile %@", self.name, self.compressorType, self.average_bitrate, self.max_bitrate, self.use_cbr, self.profile];
    
}


void PixelBufferRelease( void *releaseRefCon, const void *baseAddress )
{
    free((int *)baseAddress);
}



-(bool)compressFrame:(CapturedFrameData *)frameData
{
    
    
    if (![self hasOutputs])
    {
        return NO;
    }

    if (!_compression_session)
    {
        
        if (![self setupCompressor:frameData.videoFrame])
        {
            return NO;
        }
        return NO;
    }
    
    
    
    CFMutableDictionaryRef frameProperties;
    
    /*
    if (isKeyFrame)
    {
    
        frameProperties = CFDictionaryCreateMutable(NULL, 0, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        CFDictionaryAddValue(frameProperties, kVTEncodeFrameOptionKey_ForceKeyFrame, kCFBooleanTrue);
    } else {
     */
        frameProperties = NULL;
    //}
    
    if (!_vtpt_ref)
    {
        VTPixelTransferSessionCreate(kCFAllocatorDefault, &_vtpt_ref);
        VTSessionSetProperty(_vtpt_ref, kVTPixelTransferPropertyKey_ScalingMode, kVTScalingMode_Letterbox);
    }
    CVPixelBufferRef converted_frame;
    
    CVImageBufferRef imageBuffer = frameData.videoFrame;
    CVPixelBufferRetain(imageBuffer);

    CVPixelBufferCreate(kCFAllocatorDefault, self.working_width, self.working_height, kCVPixelFormatType_420YpCbCr8Planar, 0, &converted_frame);
    
    VTPixelTransferSessionTransferImage(_vtpt_ref, imageBuffer, converted_frame);

    //set it to nil since this is our private copy and this will force the frameData instance to release the video data
    
    frameData.videoFrame = nil;
    frameData.encoderData = converted_frame;
    
    
    CVPixelBufferRelease(imageBuffer);
    
    [self setAudioData:frameData syncObj:self];

    VTCompressionSessionEncodeFrame(_compression_session, converted_frame, frameData.videoPTS, frameData.videoDuration, frameProperties, (__bridge_retained void *)(frameData), NULL);
    
    if (frameProperties)
    {
        CFRelease(frameProperties);
    }
    
    
    return YES;
}




- (bool)setupCompressor:(CVPixelBufferRef)videoFrame
{
    OSStatus status;
    
    if (!self.settingsController)
    {
        return NO;
    }

    [self setupResolution:videoFrame];
    
    if (!self.working_height || !self.working_width)
    {
        self.errored = YES;
        return NO;
    }
    
	NSDictionary *encoderSpec = @{
		@"RequireHardwareAcceleratedVideoEncoder": @YES,
	};
	
    _compression_session = NULL;
    status = VTCompressionSessionCreate(NULL, self.working_width, self.working_height, kCMVideoCodecType_H264, (__bridge CFDictionaryRef)encoderSpec, NULL, NULL, VideoCompressorReceiveFrame,  (__bridge void *)self, &_compression_session);
    
    //CFDictionaryRef props;
    //VTCompressionSessionCopySupportedPropertyDictionary(_compression_session, &props);
    
    if (status != noErr || !_compression_session)
    {
        NSLog(@"COMPRESSOR SETUP ERROR");
        self.errored = YES;
        return NO;
    }
	
	//CFRelease(props);
    
    //If priority isn't set to -20 the framerate in the SPS/VUI section locks to 25. With -20 it takes on the value of
    //whatever ExpectedFrameRate is. I have no idea what the fuck, but it works.
    
    //VTSessionSetProperty(_compression_session, (CFStringRef)@"Priority", (__bridge CFTypeRef)(@-20));
    
    /*
	NSDictionary *transferSpec = @{
		(__bridge NSString *)kVTPixelTransferPropertyKey_ScalingMode: (__bridge NSString *)kVTScalingMode_Letterbox,
	};
    VTSessionSetProperty(_compression_session, kVTCompressionPropertyKey_PixelTransferProperties, (__bridge CFTypeRef)(transferSpec));
    */
    
    VTSessionSetProperty(_compression_session, kVTCompressionPropertyKey_AllowFrameReordering, kCFBooleanFalse);
    VTSessionSetProperty(_compression_session, (__bridge CFStringRef)@"RealTime", kCFBooleanTrue);
    
    
    
    
    
    int real_keyframe_interval = 2;
    if (self.keyframe_interval && self.keyframe_interval > 0)
    {
        real_keyframe_interval = self.keyframe_interval;
    }
    
    VTSessionSetProperty(_compression_session, kVTCompressionPropertyKey_MaxKeyFrameIntervalDuration, (__bridge CFTypeRef)@(real_keyframe_interval));
    
    
    
    

    
    int real_bitrate_limit = 0;
    float limit_seconds = 0.0f;
    
    if (self.use_cbr && self.average_bitrate && self.average_bitrate > 0)
    {
        
        limit_seconds = 1.0f;
        real_bitrate_limit = (self.average_bitrate/2)*125; // In bytes (1000/8)
        
    } else if (self.max_bitrate && self.max_bitrate > 0) {
        real_bitrate_limit = self.max_bitrate*125; // In bytes (1000/8)
        limit_seconds = 1.0f;
    }
    
    
    
    
    if (self.average_bitrate > 0)
    {
        int real_bitrate = self.average_bitrate*1000;
        
        
        VTSessionSetProperty(_compression_session, kVTCompressionPropertyKey_AverageBitRate, CFNumberCreate(NULL, kCFNumberIntType, &real_bitrate));
        
    }
    
    

    /*
    
    if (self.settingsController.captureFPS && self.settingsController.captureFPS > 0)
    {
        
        
        
        VTSessionSetProperty(_compression_session, kVTCompressionPropertyKey_ExpectedFrameRate, (__bridge CFTypeRef)(@(self.settingsController.captureFPS)));
        
    }
    
     */
    
    
    //This doesn't appear to work at all (2012 rMBP, 10.8.4). Even if you set DataRateLimits, you don't get anything back if you
    //try to retrieve it.
    
    if (real_bitrate_limit > 0)
    {
        
		NSArray *dataRateLimits = @[
			@(real_bitrate_limit),
			@(limit_seconds),
		];
        
        OSStatus success = VTSessionSetProperty(_compression_session, kVTCompressionPropertyKey_DataRateLimits, (__bridge CFTypeRef)dataRateLimits);
        if (success != noErr)
        {
            NSLog(@"FAILED TO SET DATALIMITS");
        }
    }

    
    if (self.profile)
    {
        CFStringRef session_profile = nil;
        
        
        if (floor(NSAppKitVersionNumber) <= NSAppKitVersionNumber10_8)
        {
            if ([self.profile isEqualToString:@"Baseline"])
            {
                session_profile = kVTProfileLevel_H264_Baseline_4_1;
            } else if ([self.profile isEqualToString:@"Main"]) {
                session_profile = kVTProfileLevel_H264_Main_5_0;
            } else if ([self.profile isEqualToString:@"High"]) {
                session_profile = kVTProfileLevel_H264_High_5_0;
            }            
        } else {
            if ([self.profile isEqualToString:@"Baseline"])
            {
                session_profile = (__bridge CFStringRef)@"H264_Baseline_AutoLevel";
            } else if ([self.profile isEqualToString:@"Main"]) {
                session_profile = (__bridge CFStringRef)@"H264_Main_AutoLevel";
            } else if ([self.profile isEqualToString:@"High"]) {
                session_profile = (__bridge CFStringRef)@"H264_High_AutoLevel";
            }
        }
        
        
        if (session_profile)
        {
            VTSessionSetProperty(_compression_session, kVTCompressionPropertyKey_ProfileLevel, session_profile);
        }
            
            
    }
    _audioBuffer = [[NSMutableArray alloc] init];
    return YES;
    
}


void VideoCompressorReceiveFrame(void *VTref, void *VTFrameRef, OSStatus status, VTEncodeInfoFlags infoFlags, CMSampleBufferRef sampleBuffer)
{
    
    /*
    if (VTFrameRef)
    {
        CVPixelBufferRelease(VTFrameRef);
    }
     */
    
    
    //@autoreleasepool {
        
        
        
        if(!sampleBuffer)
            return;
        
        
        
        CFRetain(sampleBuffer);
        
        CapturedFrameData *frameData;
        
        
        frameData = (__bridge_transfer CapturedFrameData *)(VTFrameRef);
        
        
        if (!frameData)
        {
            //What?
            return;
        }
        
        
        CVPixelBufferRelease(frameData.encoderData);
        

        //frameData.videoFrame = nil;
        frameData.encodedSampleBuffer = sampleBuffer;
        
        
        AppleVTCompressor *selfobj = (__bridge AppleVTCompressor *)VTref;
    
    
    
    
        for (id dKey in selfobj.outputs)
        {
            
            OutputDestination *dest = selfobj.outputs[dKey];
            
            [dest writeEncodedData:frameData];
            

        }
        
        
        CFRelease(sampleBuffer);
    //}
}


@end
