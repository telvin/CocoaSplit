//
//  CaptureController.h
//  H264Streamer
//
//  Created by Zakk on 9/2/12.
//  Copyright (c) 2012 Zakk. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <VideoToolbox/VideoToolbox.h>
#import "AVFAudioCapture.h"
#import <CoreMedia/CoreMedia.h>
#import "CSCaptureSourceProtocol.h"
#import "CSAbstractCaptureDevice.h"
#import "AppleVTCompressor.h"
#import <IOKit/pwr_mgt/IOPMLib.h>
#import <mach/mach_time.h>
#import <QuartzCore/CoreImage.h>
#import "CSPluginLoader.h"
#import "CSStreamServiceProtocol.h"
#import "CSNotifications.h"
#import "PluginManagerWindowController.h"
#import "CreateLayoutViewController.h"
#import "CAMultiAudioEngine.h"
#import "CSAnimationRunnerObj.h"
#import "CSAnimationChooserViewController.h"
#import "CSMidiManagerWindowController.h"
#import "MIKMIDI.h"
#import "CSTimerSourceProtocol.h"



@class FFMpegTask;
@protocol h264Compressor;
@class OutputDestination;
@class InputSource;
@class SourceLayout;
@class LayoutPreviewWindowController;



void VideoCompressorReceiveFrame(void *, void *, OSStatus , VTEncodeInfoFlags , CMSampleBufferRef );


@class PreviewView;


@interface CaptureController : NSObject <NSMenuDelegate, MIKMIDIMappableResponder, MIKMIDIResponder, MIKMIDIMappingGeneratorDelegate, CSTimerSourceProtocol> {
    
    bool _stagingHidden;
    
    NSArray *_inputIdentifiers;
    
    CIFilter *_compositeFilter;
    CIImage *_backgroundImage;
    
    CVDisplayLinkRef _displayLink;
    

    NSRect _stagingFrame;
    NSRect _liveFrame;
    
    
    id _audio_capture_session;
    
    CFAbsoluteTime _lastcvtime;
    
    NSThread *mainThread;
    
    NSTimer *_captureTimer;
    NSTimer *_idleTimer;
    BOOL _cmdLineInfo;
    

    NSScreen *_fullscreenOn;
    
    IOPMAssertionID _PMAssertionID;
    IOReturn _PMAssertionRet;
    
    id _activity_token;
    
    long long _frameCount;
    CFAbsoluteTime _firstFrameTime;
    CFAbsoluteTime _lastFrameTime;
    CMTime _firstAudioTime;
    dispatch_queue_t _main_capture_queue;
    dispatch_queue_t _preview_queue;
    dispatch_source_t _dispatch_timer;
    dispatch_source_t _statistics_timer;
    dispatch_source_t _audio_statistics_timer;

    
    CFAbsoluteTime _frame_interval;
    CFAbsoluteTime _staging_frame_interval;
    
    mach_timebase_info_data_t _mach_timebase;
    double _frame_time;
    double _start_time;
    
    CMSampleBufferRef audioRingBuffer[512];
    size_t audioWritePosition;
    size_t audioLastReadPosition;
    NSMutableArray *audioBuffer;
    NSMutableArray *videoBuffer;
    dispatch_source_t _log_source;
    int _saved_stderr;
    bool _last_running_value;
    
    
    CIFilter *_cifilter;
    NSOpenGLContext *_ogl_ctx;
    CGLContextObj _cgl_ctx;
    
    
    float _min_render_time;
    float _max_render_time;
    float _avg_render_time;
    float _render_time_total;
    int _renderedFrames;
    NSTask *_renderTask;
    
    NSConnection *_remoteConn;
    id _renderServer;
    NSPopover *_layoutpopOver;
    NSPopover *_animatepopOver;
    
    NSMutableArray *_screensCache;
    

    
    
}



@property (weak) IBOutlet NSMenu *stagingFullScreenMenu;
@property (weak) IBOutlet NSMenu *liveFullScreenMenu;

@property (weak) IBOutlet NSMenu *exportLayoutMenu;


@property (strong) MIKMIDIDeviceManager *midiManager;

@property (strong) NSMutableDictionary *midiDeviceMappings;

@property (strong) NSArray *midiMapGenerators;

@property (assign) NSInteger currentMidiInputStagingIdx;
@property (assign) NSInteger currentMidiInputLiveIdx;

@property (assign) bool currentMidiLayoutLive;

@property (assign) bool useMidiLiveChannelMapping;
@property (assign) NSInteger midiLiveChannel;


@property (weak) IBOutlet NSArrayController *AudioDeviceArrayController;

@property (strong) CAMultiAudioEngine *multiAudioEngine;


@property (assign) bool renderOnIntegratedGPU;


@property (strong) LayoutPreviewWindowController *layoutPreviewController;
@property (strong) PluginManagerWindowController *pluginManagerController;
@property (strong) CSMidiManagerWindowController *midiManagerController;


@property (strong) NSString *renderStatsString;


@property (strong) NSString *layoutPanelName;

@property (strong) NSMutableArray *sourceLayouts;
@property (strong) SourceLayout *selectedLayout;
@property (strong) SourceLayout *stagingLayout;

@property (weak) IBOutlet NSView *stagingControls;

@property (weak) IBOutlet NSView *goLiveControls;
@property (weak) IBOutlet NSSplitView *canvasSplitView;

@property (strong) id<h264Compressor> videoCompressor;
@property (strong) AVFAudioCapture *audioCaptureSession;


@property (assign) double captureFPS;
@property (assign) int audioBitrate;
@property (assign) int audioSamplerate;


@property (assign) double min_delay;
@property (assign) double max_delay;
@property (assign) double avg_delay;
@property (assign) long long compressedFrameCount;
@property (assign) NSMutableArray *streamPanelDestinations;
@property (strong) NSUserDefaults *cmdLineArgs;
@property (assign) double audio_adjust;

@property (assign) CFAbsoluteTime last_dl_time;

@property (weak) IBOutlet NSPopover *editorPopover;

@property (unsafe_unretained) IBOutlet PreviewView *previewCtx;
@property (weak) IBOutlet PreviewView *stagingCtx;

@property (unsafe_unretained) IBOutlet NSObjectController *objectController;
@property (strong) IBOutlet NSObjectController *compressSettingsController;
@property (strong) IBOutlet NSObjectController *outputPanelController;
@property (strong) IBOutlet NSObjectController *layoutPanelController;

@property (readonly) NSArray *layoutSortDescriptors;

@property (weak) IBOutlet PreviewView *stagingPreviewView;
@property (weak) IBOutlet PreviewView *livePreviewView;

@property (strong) NSMutableDictionary *transitionNames;
@property (strong) NSArray *transitionDirections;
@property (assign) float transitionDuration;
@property (strong) NSString *transitionName;
@property (strong) NSString *transitionDirection;
@property (strong) CIFilter *transitionFilter;

@property (strong) NSWindow *transitionFilterWindow;


-(IBAction)openTransitionFilterPanel:(NSButton *)sender;

- (IBAction)stagingViewToggle:(id)sender;
-(IBAction)doImportLayout:(id)sender;

-(void)showStagingView;

- (IBAction)openPluginManager:(id)sender;
- (IBAction)openMidiManager:(id)sender;


- (IBAction)stagingGoLive:(id)sender;
- (IBAction)stagingSave:(id)sender;
- (IBAction)stagingRevert:(id)sender;
- (IBAction)mainRevert:(id)sender;


- (IBAction)unlockStagingFPS:(id)sender;
- (IBAction)unlockLiveFPS:(id)sender;

- (void)deleteLayout:(NSInteger)deleteIdx;


- (IBAction)addStreamingService:(id)sender;

- (IBAction)streamButtonPushed:(id)sender;

- (IBAction)closeAdvancedPrefPanel:(id)sender;
- (IBAction)openAdvancedPrefPanel:(id)sender;
- (IBAction)openCreateSheet:(id)sender;
- (IBAction)openVideoAdvanced:(id)sender;
- (IBAction)closeVideoAdvanced:(id)sender;
- (void)openCompressPanel:(bool)doEdit;
- (IBAction)newCompressPanel;
- (IBAction)editCompressPanel;
-(IBAction)deleteCompressorPanel;

- (IBAction)closeCompressPanel;

- (IBAction)addInputSource:(id)sender;

- (IBAction)openAudioMixerPanel:(id)sender;
- (IBAction)closeAudioMixerPanel:(id)sender;

- (IBAction)closeCreateSheet:(id)sender;
- (IBAction)openLayoutPanel:(id)sender;


@property (strong) NSString *compressTabLabel;

@property (weak) IBOutlet NSDictionaryController *compressController;


@property (strong) id<h264Compressor> editingCompressor;
@property (strong) NSString *editingCompressorKey;
@property (strong) NSMutableDictionary *compressors;
@property (strong) id<h264Compressor> selectedCompressor;


@property (weak) NSString *selectedVideoType;
@property (strong) NSString *selectedCompressorType;


@property (strong) NSArray *videoTypes;
@property (strong) NSArray *compressorTypes;


@property (strong) NSMutableArray *ffmpeg_objects;
@property (weak) NSString *streamingServiceServer;
@property (weak) NSString *streamingServiceKey;

@property (weak) NSString *streamingDestination;


@property (weak) NSString *selectedDestinationType;

@property (weak) IBOutlet NSTabView *compressTabs;
@property (strong) IBOutlet NSWindow *createSheet;
@property (strong) IBOutlet NSWindow *advancedVideoPanel;
@property (strong) IBOutlet NSWindow *compressPanel;
@property (strong) IBOutlet NSWindow *advancedPrefPanel;
@property (strong) IBOutlet NSWindow *logWindow;
@property (strong) IBOutlet NSWindow *audioMixerPanel;
@property (strong) IBOutlet NSWindow *outputEditPanel;
@property (strong) IBOutlet NSWindow *layoutPanel;

@property (weak) IBOutlet NSView *streamServiceAddView;

@property (unsafe_unretained) IBOutlet NSWindow *streamServiceConfWindow;
@property (strong) NSViewController *streamServicePluginViewController;
@property (strong) NSObject<CSStreamServiceProtocol>*streamServiceObject;


- (IBAction)openLogWindow:(id)sender;


@property (readonly) NSArray *destinationTypes;

@property (strong) NSMutableArray *captureDestinations;
@property (weak) NSIndexSet *selectedCaptureDestinations;
@property (assign) int selectedTabIndex;


@property (assign) BOOL showPreview;

@property (assign) int captureVideoAverageBitrate;
@property (assign) int captureVideoMaxBitrate;
@property (assign) int captureVideoMaxKeyframeInterval;
@property (strong) NSString *x264tune;
@property (strong) NSString *x264preset;
@property (strong) NSString *x264profile;
@property (assign) int x264crf;
@property (strong) NSMutableArray *x264tunes;
@property (strong) NSMutableArray *x264presets;
@property (strong) NSMutableArray *x264profiles;
@property (strong) NSString *vtcompressor_profile;
@property (assign) BOOL videoCBR;

@property (assign) int maxOutputPending;
@property (assign) int maxOutputDropped;

@property (assign) BOOL captureRunning;
@property (strong) NSArray *arOptions;
@property (strong) NSString *resolutionOption;

@property (strong) OutputDestination *editDestination;





@property (assign) int captureHeight;
@property (assign) int captureWidth;


@property (strong) NSArray *validSamplerates;



- (IBAction)removeDestination:(id)sender;

@property (weak) NSArray *audioCaptureDevices;

@property (strong) FFMpegTask *ffmpeg_obj;
@property (strong) AVAssetWriterInput *video_writer;
@property (strong) AVAssetWriterInput *audio_writer;


@property (weak) CSAbstractCaptureDevice *selectedVideoCapture;
@property (readonly) AVCaptureDevice *selectedAudioCapture;


@property (weak)  NSString *ffmpeg_path;

@property NSString *imageDirectory;

@property (strong) NSDictionary *extraSaveData;

@property (strong) NSPipe *loggingPipe;
@property (strong) NSFileHandle *logReadHandle;

@property (assign) bool useStatusColors;

@property (unsafe_unretained) IBOutlet NSTextView *logTextView;

@property (weak) IBOutlet NSMenu *extrasMenu;

@property (strong) NSMutableDictionary *extraPlugins;

@property (strong) NSMutableDictionary *extraPluginsSaveData;
@property (strong) CSPluginLoader *sharedPluginLoader;



- (IBAction)openAnimatePopover:(NSButton *)sender;

- (void)saveSettings;
- (void)loadSettings;
- (bool) startStream;
- (void) stopStream;
- (void) loadCmdlineSettings:(NSUserDefaults *)cmdargs;
-(void)setExtraData:(id)saveData forKey:(NSString *)forKey;
-(id)getExtraData:(NSString *)forkey;
-(CVPixelBufferRef)currentFrame;
-(double)mach_time_seconds;
-(bool)pendingStreamConfirmation:(NSString *)queryString;
-(int)streamsPendingCount;
-(int)streamsActiveCount;
-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;
-(NSColor *)statusColor;
-(void)captureOutputAudio:(id)fromDevice didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer;
-(void)deleteSource:(InputSource *)delSource;
-(InputSource *)findSource:(NSPoint)forPoint;
-(SourceLayout *)addLayoutFromBase:(SourceLayout *)baseLayout;
-(SourceLayout *)getLayoutForName:(NSString *)name;

- (IBAction)layoutTableSelected:(NSTableView *)sender;
- (IBAction)openLayoutPopover:(NSButton *)sender;
- (IBAction)mainDeleteLayoutClicked:(id)sender;
- (IBAction)mainCopyLayoutClicked:(id)sender;
-(IBAction)stagingDeleteLayoutClicked:(id)sender;
-(IBAction)stagingCopyLayoutClicked:(id)sender;

- (IBAction)stagingAnimationSelected:(id)sender;


@property (weak) IBOutlet NSArrayController *sourceLayoutsArrayController;
@property (weak) IBOutlet NSTableView *mainSourceLayoutTableView;
@property (weak) IBOutlet NSTableView *stagingSourceLayoutTableView;

-(void)setupLogging;
+(CSAnimationRunnerObj *) sharedAnimationObj;
- (NSArray *)commandIdentifiers;
- (MIKMIDIResponderType)MIDIResponderTypeForCommandIdentifier:(NSString *)commandID;
-(void)learnMidiForCommand:(NSString *)command withRepsonder:(id<MIKMIDIMappableResponder>)responder;

-(void)openMidiLearnerForResponders:(NSArray *)responders;
-(void)clearLearnedMidiForCommand:(NSString *)command withResponder:(id<MIKMIDIMappableResponder>)responder;



-(void)layoutWentFullscreen;
-(void)layoutLeftFullscreen;
+(void)loadPythonClass:(NSString *)pyClass fromFile:(NSString *)fromFile withBlock:(void(^)(Class))withBlock;
+(Class)loadPythonClass:(NSString *)pyClass fromFile:(NSString *)fromFile;






@end
