//
//  MovieCaptureViewController.h
//  CocoaSplit
//
//  Created by Zakk on 8/28/14.
//  Copyright (c) 2014 Zakk. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MovieCapture.h"

@interface MovieCaptureViewController : NSViewController

@property (strong) NSDictionary *repeatTypeMap;
@property (strong) NSArray *repeatSortDescriptors;

@property (weak) MovieCapture *captureObj;

@end
