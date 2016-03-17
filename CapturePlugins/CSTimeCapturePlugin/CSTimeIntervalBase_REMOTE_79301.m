//
//  CSTimeIntervalBase.m
//  CSTimeCapturePlugin
//
//  Created by Zakk on 2/7/15.
//  Copyright (c) 2015 Zakk. All rights reserved.
//

#import "CSTimeIntervalBase.h"

@implementation CSTimeIntervalBase
@synthesize format = _format;
@synthesize paused = _paused;


-(instancetype)init
{
    if (self = [super init])
    {
        self.styleTypeMap = @{@"None": @(NSTimeIntervalFormatterNoStyle),
                              @"Short": @(NSTimeIntervalFormatterShortStyle),
                              @"Medium": @(NSTimeIntervalFormatterMediumStyle),
                              @"Long": @(NSTimeIntervalFormatterLongStyle),
                              @"Full": @(NSTimeIntervalFormatterFullStyle)
                              };
        self.formatter = [[NSTimeIntervalFormatter alloc] init];
        
        self.format = @"MM:ss.SS";
        
        //[self.formatter setFormat:NSTimeIntervalFormatterMediumStyle];
        
    }
    return self;
}

-(instancetype)initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder])
    {
        self.endDate = [aDecoder decodeObjectForKey:@"endDate"];
        self.startDate = [aDecoder decodeObjectForKey:@"startDate"];
        self.format = [aDecoder decodeObjectForKey:@"format"];
        self.paused = [aDecoder decodeBoolForKey:@"paused"];
    }
    
    return self;
}

-(void)encodeWithCoder:(NSCoder *)aCoder
{
    [super encodeWithCoder:aCoder];
    [aCoder encodeObject:self.endDate forKey:@"endDate"];
    [aCoder encodeObject:self.startDate forKey:@"startDate"];
    [aCoder encodeObject:self.format forKey:@"format"];
    [aCoder encodeBool:self.paused forKey:@"paused"];

    
}


-(void)setFormat:(NSString *)format
{
    _format = format;
    [self.formatter setFormat:format];
}

-(NSString *)format
{
    return _format;
}


-(NSArray *)styleTypes
{
    return _styleTypeMap.allKeys;
}



-(void)frameTick
{
    NSTimeInterval interval;
    if (self.paused)
    {
        return;
    }
    
    //NSDate *newNow = [NSDate date];
    if (self.startDate) {
        interval = -[self.startDate timeIntervalSinceNow];
    } else if (self.endDate) {
        interval = [self.endDate timeIntervalSinceNow];
    }
    //self.startDate = newNow;
    
    if (interval < 0)
    {
        interval = 0;
    }
    
    self.text = [self.formatter stringFromInterval:interval];
    
}


@end
