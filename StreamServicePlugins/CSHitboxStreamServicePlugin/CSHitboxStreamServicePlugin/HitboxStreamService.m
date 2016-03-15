//
//  HitboxStreamService.m
//  CSHitboxStreamServicePlugin
//
//  Created by Zakk on 12/1/14.
//  Copyright (c) 2014 Zakk. All rights reserved.
//

#import "HitboxStreamService.h"
#import "HitboxStreamServiceViewController.h"


@implementation HitboxStreamService


-(instancetype) init
{
    if (self = [super init])
    {
        self.isReady = YES;
    }
    return self;
}

+(NSString *)label
{
    return @"Hitbox";
}

+(NSString *)serviceDescription
{
    return @"Hitbox";
}


-(NSString *)getServiceDestination
{
    
    
    
    if (self.selectedServer)
    {
        NSString *destination = [NSString stringWithFormat:@"%@/%@", self.selectedServer, self.streamPath];
        
        return destination;
    }
    
    return nil;
}

-(NSViewController *)getConfigurationView
{
    
    HitboxStreamServiceViewController *configViewController;
    
    configViewController = [[HitboxStreamServiceViewController alloc] initWithNibName:@"HitboxStreamServiceViewController" bundle:[NSBundle bundleForClass:self.class]];
    configViewController.serviceObj = self;
    return configViewController;
}


-(void)fetchIngestServers:(void(^)(void))callback
{
    NSString *urlString = [NSString stringWithFormat:@"%@streamingest/%@?authToken=%@", @HITBOX_API_BASE, self.authUsername, self.authKey];
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:urlString]];
    
    [NSURLConnection sendAsynchronousRequest:request queue:[NSOperationQueue mainQueue] completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) {
        
        NSError *jsonError;
        NSDictionary *ingest_response = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&jsonError];
        
        
        self.streamKey = [ingest_response objectForKey:@"stream_key"];
        self.streamPath = [ingest_response objectForKey:@"stream_path"];
        
        self.ingestServers = [ingest_response objectForKey:@"stream_ingest_list"];
        if (callback)
        {
            callback();
        }
        
    }];
}


-(void)authenticate:(NSString *)username password:(NSString *)password onComplete:(void(^)(void))callback
{
    NSString *urlString = [NSString stringWithFormat:@"%@auth/token", @HITBOX_API_BASE];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:urlString]];
    
    
    request.HTTPMethod = @"POST";
    
    NSString *authData = [NSString stringWithFormat:@"login=%@&pass=%@&app=desktop", username, password];
    request.HTTPBody = [authData dataUsingEncoding:NSUTF8StringEncoding];

    [NSURLConnection sendAsynchronousRequest:request queue:[NSOperationQueue mainQueue] completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) {
        NSError *jsonError;
        
        NSDictionary *auth_response = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&jsonError];
        
        
        
        NSString *authToken = [auth_response objectForKey:@"authToken"];
        
        self.authKey = authToken;
        self.authUsername = username;
        if (callback)
        {
            callback();
        }
    }];
}


@end
