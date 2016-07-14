/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#import "AJCPSActionDialogCell.h"

static const float NUM_ACTIONS = 3;

@implementation AJCPSActionDialogCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.cpsButton = [UIButton buttonWithType:UIButtonTypeSystem];

        [self.cpsButton setFrame:CGRectMake(10, 0, 300, 60)];
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

- (void)touchUpInsideAction:(id)sender
{
    AJCPSDialog *childDialog = [self.actionDialogWidget getChildDialog];

    NSString *message = [childDialog getMessage];
    uint16_t numActions = [childDialog getNumActions];

    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"" message:message preferredStyle:UIAlertControllerStyleAlert];

    for (uint16_t i = 1; i <= numActions; ++i) {
        SEL getLabel = NSSelectorFromString([NSString stringWithFormat:@"getLabelAction%d", i]);
        NSString *label = [childDialog performSelector:getLabel];

        UIAlertAction *action = [UIAlertAction actionWithTitle:label style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
                                     SEL executeAction = NSSelectorFromString([NSString stringWithFormat:@"executeAction%d", i]);
                                     [childDialog performSelector:executeAction];
                                 }];

        [alertController addAction:action];
    }

    [self.viewController presentViewController:alertController animated:YES completion:nil];
}

- (void)setActionDialogWidget:(AJCPSActionWithDialog *)actionDialogWidget
{
    _actionDialogWidget = actionDialogWidget;
    [self.cpsButton setTitle:[self.actionDialogWidget getLabel] forState:UIControlStateNormal];
    [self.cpsButton setEnabled:[self.actionDialogWidget getIsEnabled] ? YES : NO];
}

- (void)setViewController:(AJCPSGetControlPanelViewController *)viewController
{
    _viewController = viewController;
}

@end
