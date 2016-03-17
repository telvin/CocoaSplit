//
//  CSSyphonCaptureLayer.h
//  CSSyphonCapturePlugin
//
//  Created by Zakk on 2/16/15.
//  Copyright (c) 2015 Zakk. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
<<<<<<< HEAD
#import <SyphonBuildMacros.h>
#import "Syphon.h"
    //#import <Syphon.h>
=======
#import "SyphonBuildMacros.h"
#import "Syphon.h"
>>>>>>> 023b77e2f8fce65605350a6361b6f79c157d2b55

@interface CSSyphonCaptureLayer : CAOpenGLLayer
{
    CGLContextObj _myCGLContext;
    CGRect _lastBounds;
    CGSize _lastImageSize;
    CGRect _privateCropRect;
    CGRect _lastCrop;
    CGRect _calculatedCrop;
    bool _needsRedraw;
    CFAbsoluteTime _lastDrawTime;
}


@property (strong) SyphonClient *syphonClient;
@property (assign) bool flipImage;

@end
