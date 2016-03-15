//
//  SourceLayout.h
//  CocoaSplit
//
//  Created by Zakk on 8/31/14.
//  Copyright (c) 2014 Zakk. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InputSource.h"
#import "CSNotifications.h"
#import <malloc/malloc.h>
#import "CSAnimationItem.h"
#import "CSAnimationRunnerObj.h"
#import "MIKMIDI.h"



@interface SourceLayout : NSObject <NSCoding, NSKeyedUnarchiverDelegate, NSKeyedArchiverDelegate, NSCopying, MIKMIDIMappableResponder, MIKMIDIResponder>
{
    
    NSSortDescriptor *_sourceDepthSorter;
    NSSortDescriptor *_sourceUUIDSorter;
    CVPixelBufferPoolRef _cvpool;
    CVPixelBufferRef _currentPB;
    NSSize _rootSize;
    GLuint _fboTexture;
    GLuint _rFbo;
    dispatch_queue_t _animationQueue;
    NSMutableDictionary *_uuidMap;
    
    
}


@property (assign) bool inTransition;
@property (strong) NSMutableArray *animationList;
@property (strong) NSIndexSet *animationIndexes;

@property (strong) CSAnimationItem *selectedAnimation;

@property (strong) NSMutableDictionary *animationSaveData;


@property (strong) NSMutableArray *sourceList;
@property (strong) NSData *savedSourceListData;
@property (assign) bool isActive;


@property (assign) int canvas_width;
@property (assign) int canvas_height;

@property (assign) float frameRate;

@property (assign) CGLContextObj cglCtx;

@property (strong) NSString *name;

@property (strong) CARenderer *renderer;
@property (strong) CALayer *rootLayer;

@property (strong) SourceCache *sourceCache;
@property (strong) CIFilter *compositeFilter;

@property (weak) InputSource *layoutTimingSource;
@property (strong) NSUndoManager *undoManager;


-(void)deleteSource:(InputSource *)delSource;
-(void)addSource:(InputSource *)newSource;
-(InputSource *)findSource:(NSPoint)forPoint deepParent:(bool)deepParent;
-(InputSource *)findSource:(NSPoint)forPoint withExtra:(float)withExtra deepParent:(bool)deepParent;
-(NSArray *)sourceListOrdered;
-(void) saveSourceList;
-(void)restoreSourceList:(NSData *)withData;
-(void) saveAnimationSource;
-(NSData *)makeSaveData;

-(InputSource *)inputForUUID:(NSString *)uuid;
-(void)frameTick;
-(NSObject *)mergeSourceListData:(NSData *)mergeData withLayer:(CALayer *)withLayer;
-(IBAction)runAnimations:(id)sender;
-(void)addAnimation:(NSDictionary *)animation;
-(InputSource *)sourceUnder:(InputSource *)source;
-(void)didBecomeVisible;
-(bool)containsInput:(InputSource *)cSource;
-(void)modifyUUID:(NSString *)uuid withBlock:(void (^)(InputSource *input))withBlock;




@end
