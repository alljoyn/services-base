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

#import "AJCPSCPSButtonCell.h"
#import "AJCPSAction.h"

@implementation CPSButtonCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.cpsButton = [UIButton buttonWithType:UIButtonTypeSystem];
        
        [self.cpsButton setFrame:CGRectMake(10,0,300,60)];
        [self.cpsButton.titleLabel setFont:[UIFont systemFontOfSize:13]];
        [self.cpsButton addTarget:self action:@selector(touchUpInsideAction:) forControlEvents:UIControlEventTouchUpInside];
        [self.contentView addSubview:self.cpsButton];
        [self reloadInputViews];
    }
    return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
}

- (void)touchUpInsideAction:(id)sender {
    
    NSLog(@"Pressed %@",[self.cpsButton titleLabel].text);
    
    QStatus status = [((AJCPSAction *)self.actionWidget) executeAction];
    if (status != ER_OK) {
        NSLog(@"execute Action returned error %d, %@",status, [AJNStatus descriptionForStatusCode:status]);
    }
}

-(void)setActionWidget:(AJCPSAction *)actionWidget
{
    _actionWidget = actionWidget;
    [self.cpsButton setTitle:[self.actionWidget getLabel] forState:UIControlStateNormal];
    [self.cpsButton setEnabled:[self.actionWidget getIsEnabled]?YES:NO];
    // We do not use [self.actionWidget getBgColor] so the iOS look and feel remain the same
    
}

@end