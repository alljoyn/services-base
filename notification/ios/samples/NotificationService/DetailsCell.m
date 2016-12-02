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

#import "DetailsCell.h"

@implementation DetailsCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {

    }
    return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
    
    if ([self.notificationEntry dismissed]) {
        [self setAccessoryType:UITableViewCellAccessoryNone];
        [self.notificationEntry setChosen:NO];
        return;
    }

    if(selected)
    {
        if (self.accessoryType == UITableViewCellAccessoryNone) {
            [self setAccessoryType:UITableViewCellAccessoryCheckmark];
            [self.notificationEntry setChosen:YES];
        } else {
            [self setAccessoryType:UITableViewCellAccessoryNone];
            [self.notificationEntry setChosen:NO];
        }
    }
}



-(void)setNotificationEntry:(NotificationEntry *)notificationEntry
{
    _notificationEntry = notificationEntry;
    
    self.detailsLabel.text = notificationEntry.text;
    
    if([self.notificationEntry chosen])
    {
        [self setAccessoryType:UITableViewCellAccessoryCheckmark];
    } else {
        [self setAccessoryType:UITableViewCellAccessoryNone];
    }
    
    if([self.notificationEntry dismissed])
    {
        [self setBackgroundColor:[UIColor yellowColor]];
    } else {
        [self setBackgroundColor:[UIColor whiteColor]];
    }
}

@end