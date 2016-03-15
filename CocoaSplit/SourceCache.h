//
//  SourceCache.h
//  CocoaSplit
//
//  Created by Zakk on 8/30/14.
//  Copyright (c) 2014 Zakk. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CSCaptureSourceProtocol.h"


@interface SourceCache : NSObject


@property (strong) NSMapTable *cacheMap;

+(id) sharedCache;

-(id) cacheSource:(NSObject<CSCaptureSourceProtocol>*)ofType uniqueID:(NSString *)uniqueID;


@end
