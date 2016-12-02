/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#import "AnnounceTextViewController.h"
#import "alljoyn/about/AJNAboutDataConverter.h"

@interface AnnounceTextViewController ()

@property (weak, nonatomic) IBOutlet UITextView *announceInformation;

@end

@implementation AnnounceTextViewController

- (void)viewDidLoad
{
	[super viewDidLoad];
    
	//  Retrive AJNAnnouncement by the  announcementButtonCurrentTitle unique name
	NSString *txt = [[NSString alloc] init];
    
	// Set Announcement title
	NSString *title = [self.ajnAnnouncement busName];
    
	txt = [txt stringByAppendingFormat:@"%@\n%@\n", title, [@"" stringByPaddingToLength :[title length] + 10 withString : @"-" startingAtIndex : 0]];
    
	// Set Announcement body
	txt = [txt stringByAppendingFormat:@"BusName: %@\n", [self.ajnAnnouncement busName]];
    
	txt = [txt stringByAppendingFormat:@"Port: %hu\n", [self.ajnAnnouncement port]];
    
    txt = [txt stringByAppendingFormat:@"Version: %u\n", [self.ajnAnnouncement version]];
    
	txt = [txt stringByAppendingString:@"\n\n"];
    
	// Set Announcement AboutMap data
	txt = [txt stringByAppendingFormat:@"About map:\n"];
    
	txt = [txt stringByAppendingString:[AJNAboutDataConverter aboutDataDictionaryToString:([self.ajnAnnouncement aboutData])]];
    
	txt = [txt stringByAppendingString:@"\n\n"];
    
	//  Set Announcement ObjectDesc data
	txt = [txt stringByAppendingFormat:@"Bus Object Description:\n"];
	txt = [txt stringByAppendingString:[AJNAboutDataConverter objectDescriptionsDictionaryToString:[self.ajnAnnouncement objectDescriptions]]];
    
	self.announceInformation.text = txt;
}

@end