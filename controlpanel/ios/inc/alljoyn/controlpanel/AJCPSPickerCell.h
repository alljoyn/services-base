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

#import <UIKit/UIKit.h>
#import "AJCPSWidget.h"
#import "AJCPSProperty.h"
#import "AJCPSControllerModel.h"

@interface AJCPSPickerCell : UITableViewCell <UIPickerViewDataSource, UIPickerViewDelegate>
@property (strong, nonatomic) UIPickerView *picker;
@property (strong, nonatomic) NSMutableArray *pickerOptions;
@property (strong, nonatomic) UILabel *label;
@property (strong, nonatomic) UITextField *pickerTextField;
@property (weak, nonatomic) AJCPSWidget *widget;
@end