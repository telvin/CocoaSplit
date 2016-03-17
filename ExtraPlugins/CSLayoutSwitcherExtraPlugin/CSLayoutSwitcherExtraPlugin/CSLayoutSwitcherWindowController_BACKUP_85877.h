//
//  CSLayoutSwitcherWindowController.h
//  CSLayoutSwitcherExtraPlugin
//
//  Created by Zakk on 9/5/14.
//  Copyright (c) 2014 Zakk. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "CSLayoutSwitchAction.h"

@class CSLayoutSwitcher;




@interface CSLayoutSwitcherWindowController : NSWindowController

@property (weak) CSLayoutSwitcher *layoutSwitcher;

@property (assign) layout_switch_event eventType;
@property (strong) NSString *applicationString;
@property (strong) NSString *layoutName;
@property (readonly) NSArray *layoutNames;

@property (strong) IBOutlet NSArrayController *switchActionsController;

- (IBAction)addSwitchEvent:(id)sender;

- (IBAction)deleteLayoutActions:(id)sender;

@end
