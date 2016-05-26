/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#import "AJCPSActionDialogCell.h"

static const float NUM_ACTIONS = 3;

@implementation AJCPSActionDialogCell

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

        if ([UIAlertController class]) {
            self.alertActionDict = nil;
        }
        else {
            self.alertActionDict = [NSMutableDictionary dictionaryWithCapacity:NUM_ACTIONS];
        }
    }
    return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
}

- (void)touchUpInsideAction:(id)sender
{
    AJCPSDialog* childDialog = [self.actionDialogWidget getChildDialog];
    
    NSString *message = [childDialog getMessage];
    uint16_t numActions = [childDialog getNumActions];

    if ([UIAlertController class]) {
        UIAlertController* alertController = [UIAlertController alertControllerWithTitle:@"" message:message preferredStyle:UIAlertControllerStyleAlert];

        for(uint16_t i = 1; i <= numActions; ++i){
            SEL getLabel = NSSelectorFromString([NSString stringWithFormat:@"getLabelAction%d", i]);
            NSString *label = [childDialog performSelector:getLabel];

            UIAlertAction* action = [UIAlertAction actionWithTitle:label style: UIAlertActionStyleDefault handler:^(UIAlertAction *action){
                SEL executeAction = NSSelectorFromString([NSString stringWithFormat:@"executeAction%d", i]);
                [childDialog performSelector:executeAction];
            }];

            [alertController addAction:action];
        }

        [self.viewController presentViewController:alertController animated:YES completion:nil];
    } else {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@""
                                                            message:message
                                                           delegate:self
                                                  cancelButtonTitle:nil
                                                  otherButtonTitles:nil];

        for(uint16_t i = 1; i <= numActions; ++i) {
            SEL getLabel = NSSelectorFromString([NSString stringWithFormat:@"getLabelAction%d", i]);
            NSString *label = [childDialog performSelector:getLabel];
            [alertView addButtonWithTitle:label];

            NSString *action = [NSString stringWithFormat:@"executeAction%d", i];
            [self.alertActionDict setValue:action forKey:label];
        }

        [alertView show];
    }
}

-(void)setActionDialogWidget:(AJCPSActionWithDialog *)actionDialogWidget
{
    _actionDialogWidget = actionDialogWidget;
    [self.cpsButton setTitle:[self.actionDialogWidget getLabel] forState:UIControlStateNormal];
    [self.cpsButton setEnabled:[self.actionDialogWidget getIsEnabled]?YES:NO];
}

-(void)setViewController:(AJCPSGetControlPanelViewController *)viewController
{
    _viewController = viewController;
}

- (void)alertView:(UIAlertView *)theAlert clickedButtonAtIndex:(NSInteger)buttonIndex
{
    NSString *buttonTitle = [theAlert buttonTitleAtIndex:buttonIndex];
    NSLog(@"Button '%@' clicked", buttonTitle);

    NSString *action = self.alertActionDict[buttonTitle];
    NSLog(@"Action retrieved from dict is '%@'", action);

    if(action) {
        SEL executeAction = NSSelectorFromString(action);
        AJCPSDialog* childDialog = [self.actionDialogWidget getChildDialog];
        [childDialog performSelector:executeAction];
    }
}

@end