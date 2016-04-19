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

#import "AJCPSCPSPickerCell.h"
#import "AJCPSConstraintList.h"

@implementation CPSPickerCell

const NSInteger NUM_COLUMNS = 1;
const CGFloat COMPONENT_WIDTH = 300;
const CGFloat COMPONENT_HEIGHT = 40;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.label = [[UILabel alloc] initWithFrame:CGRectMake(10, 0, 310, 20)];
        [self.contentView addSubview:self.label];
        
        self.pickerTextField = [[UITextField alloc] initWithFrame:CGRectMake(10, 20, 300, 40)];
        
        self.picker = [[UIPickerView alloc] init];
        self.picker.delegate = self;
        self.picker.dataSource = self;
        self.picker.showsSelectionIndicator = YES;
        self.picker.hidden = NO;
        
        self.pickerTextField.inputView = self.picker;
        self.pickerTextField.borderStyle = UITextBorderStyleRoundedRect;
        
        [self.contentView addSubview:self.pickerTextField];
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
    self.label.text  = [NSString stringWithFormat:@"%@",[self.widget getLabel]];
    
    NSLog(@"widget is: %@", self.widget);
    NSArray *list = [((AJCPSProperty *)self.widget) getConstraintList];
    
    self.pickerOptions = [self buildPickerOptions:list];
    NSLog(@"widget constraint list is: %@", self.pickerOptions);
    
    if ([self.pickerTextField.text length] <= 0){
         self.pickerTextField.text = self.pickerOptions[0];
    }
}

- (void)setPickerOptions:(NSMutableArray *)pickerOptions
{
    _pickerOptions = pickerOptions;
    [self.picker reloadAllComponents];
}

- (NSMutableArray *)buildPickerOptions:(NSArray *)list
{
    NSMutableArray *options = [[NSMutableArray alloc]init];
    
    if ([list count] <= 0) {
        NSLog(@"Building picker options failed due to empty list: %@", list);
        return nil;
    }
    
    NSLog(@"Building picker options for list: %@", list);
    for (AJCPSConstraintList *constraintList in list) {
        NSMutableString *str = [[NSMutableString alloc] init];
        [str appendFormat:@"%@",[constraintList getDisplay]];
        
        AJCPSConstraintValue propertyValue = [constraintList getConstraintValue];
        AJCPSPropertyType propertyType = [constraintList getPropertyType];
        [str appendFormat:@"%@", [constraintList propertyToNSString:propertyType withValue:propertyValue]];
        
        NSLog(@"'%@' added to options list", str);
        [options addObject:str];
    }
    
    NSLog(@"picker options built: %@", options);
    return options;
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return NUM_COLUMNS;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return [self.pickerOptions count];
}

- (NSString*)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    if ([self.pickerOptions count] > 0) {
        return self.pickerOptions[row];
    }
    return nil;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    NSArray *list = [((AJCPSProperty *)self.widget) getConstraintList];
    for (AJCPSConstraintList *constraintList in list) {
        NSMutableString *str = [[NSMutableString alloc] init];
        [str appendFormat:@"%@",[constraintList getDisplay]];
        
        AJCPSConstraintValue propertyValue = [constraintList getConstraintValue];
        AJCPSPropertyType propertyType = [constraintList getPropertyType];
        [str appendFormat:@"%@", [constraintList propertyToNSString:propertyType withValue:propertyValue]];
        
        NSString *pickerName = self.pickerOptions[row];
        if ([pickerName isEqualToString:str]){
            AJCPSProperty *property = (AJCPSProperty *)self.widget;
            [property setValueFromUnsignedShort:propertyValue.uint16Value];
            break;
        }

    }
    self.pickerTextField.text = self.pickerOptions[row];
    [self.pickerTextField resignFirstResponder];
}

-(CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component
{
    return COMPONENT_WIDTH;
}

-(CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
    return COMPONENT_HEIGHT;
}

@end