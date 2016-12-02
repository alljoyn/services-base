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

#import "AJCPSLabelCell.h"

@implementation AJCPSLabelCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.widgetNameLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, 0, 310, 20)];
        [self.contentView addSubview:self.widgetNameLabel];
    }
    return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
}

- (void)setWidget:(AJCPSWidget *)widget
{
    _widget = widget;
    self.widgetNameLabel.text = [NSString stringWithFormat:@"%@", [self.widget getLabel]];
}

@end