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

#import "AJSVCGenericLoggerDefaultImpl.h"
#import "AJSVCGenericLoggerAdapter.h"
#import "AJSVCGenericLoggerUtil.h"

@interface AJSVCGenericLoggerDefaultImpl ()
@property QLogLevel currentlogLevel;
@end

@implementation AJSVCGenericLoggerDefaultImpl

- (id)init
{
    self = [super init];
    if (self) {
        self.currentlogLevel = QLEVEL_DEBUG;
        [self printTag:[NSString stringWithFormat:@"%@", [[self class] description]]
         text:@"Logger Started" logLevel:self.currentlogLevel];

        [self printTag:[NSString stringWithFormat:@"%@", [[self class] description]]
         text:[NSString stringWithFormat:@"App Version:%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"]]
         logLevel:self.currentlogLevel];
    }
    return self;
}

- (void)debugTag:(NSString *)tag text:(NSString *)text
{
    [self printTag:tag text:text logLevel:QLEVEL_DEBUG];
}

- (void)infoTag:(NSString *)tag text:(NSString *)text
{
    [self printTag:tag text:text logLevel:QLEVEL_INFO];
}

- (void)warnTag:(NSString *)tag text:(NSString *)text
{
    [self printTag:tag text:text logLevel:QLEVEL_WARN];
}

- (void)errorTag:(NSString *)tag text:(NSString *)text
{
    [self printTag:tag text:text logLevel:QLEVEL_ERROR];
}

- (void)fatalTag:(NSString *)tag text:(NSString *)text
{
    [self printTag:tag text:text logLevel:QLEVEL_FATAL];
}

- (void)printTag:(NSString *)tag text:(NSString *)logText logLevel:(QLogLevel)functionLogLevel
{
    // Get the logger level and print logs accordingly
    QLogLevel loggerLogLevel = [self logLevel];

    if (functionLogLevel <= loggerLogLevel) {
        NSLog(@"[%@][%@] %@", [AJSVCGenericLoggerUtil toStringQLogLevel:functionLogLevel], tag, logText);
    }
}

- (void)setLogLevel:(QLogLevel)logLevel
{
    [self printTag:[NSString stringWithFormat:@"%@", [[self class] description]] text:[NSString stringWithFormat:@"New Log level is %@.", [AJSVCGenericLoggerUtil toStringQLogLevel:logLevel]] logLevel:self.currentlogLevel];

    self.currentlogLevel = logLevel;
}

- (QLogLevel)logLevel
{
    return self.currentlogLevel;
}

@end