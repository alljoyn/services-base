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


#import "NotificationEntry.h"
#import "NotificationUtils.h"


@interface NotificationEntry ()
@property (strong, nonatomic) AJNSNotification *ajnsNotification;
@property (weak,nonatomic) ConsumerViewController *consumerViewController;

@end

@implementation NotificationEntry

- (id)initWithAJNSNotification:(AJNSNotification *) ajnsNotification andConsumerViewController:(ConsumerViewController *)consumerViewController
{
    self = [super init];
    if (self) {
        self.consumerViewController = consumerViewController;
        self.ajnsNotification = ajnsNotification;
    }
    return self;
}


-(void)setAjnsNotification:(AJNSNotification *)ajnsNotification
{
  
    _ajnsNotification = ajnsNotification;
  
    //show AJNSNotification on self.notificationTextView
    NSString *nstr = @"";
    NSString *lang;
    bool matchLangFlag = false;
    
    nstr = [nstr stringByAppendingString:[AJNSNotificationEnums AJNSMessageTypeToString:[ajnsNotification messageType]]];
    nstr = [nstr stringByAppendingFormat:@" "];
    
    // get the msg NotificationText

    for (AJNSNotificationText *nt in ajnsNotification.ajnsntArr) {
        lang = [nt getLanguage];
        if ([lang isEqualToString:self.consumerViewController.consumerLang]) {
            nstr = [nstr stringByAppendingString:lang];
            nstr = [nstr stringByAppendingFormat:@": "];
            nstr = [nstr stringByAppendingString:[nt getText]];
            nstr = [nstr stringByAppendingFormat:@" "];
            matchLangFlag = true;
        }
    } //for
    
    if (!matchLangFlag) {
        nstr = [nstr stringByAppendingString:[NSString stringWithFormat:@"\nUnknown language(s) in notification,\n the last one was '%@'",lang]];
        self.text = nstr;
        [self.consumerViewController.logger debugTag:[[self class] description] text:@"The received message lang does not match the consumer lang settings"];
        return;
    }
    
    // get the msg NotificationText
    NSString *richIconUrl = [ajnsNotification richIconUrl];
    if ([NotificationUtils textFieldIsValid:richIconUrl]) {
        nstr = [nstr stringByAppendingFormat:@"\nicon: %@", richIconUrl];
    }
    
    // get the msg richAudioUrl
    __strong NSMutableArray *richAudioUrlArray = [[NSMutableArray alloc] init];
    
    [ajnsNotification richAudioUrl:richAudioUrlArray];
    int i = 0;
    for (int x = 0; x != [richAudioUrlArray count]; x++) {
        [self.consumerViewController.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"%d", i]];
        i++;
        NSString *lang = [(AJNSRichAudioUrl *)[richAudioUrlArray objectAtIndex:x] language];
        NSString *audiourl = [(AJNSRichAudioUrl *)[richAudioUrlArray objectAtIndex:x] url];
        [self.consumerViewController.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"lang[%@]", lang]];
        [self.consumerViewController.logger debugTag:[[self class] description] text:[NSString stringWithFormat:@"audiourl[%@]", audiourl]];
        if ([lang isEqualToString:(self.consumerViewController.consumerLang)]) {
            nstr = [nstr stringByAppendingFormat:@"\naudio: %@", audiourl];
        }
    } //for
    
    [self.consumerViewController.logger debugTag:[[self class] description] text:([NSString stringWithFormat:@"Adding notification to view:\n %@", nstr])];
    
    
    self.text = nstr;
}

@end