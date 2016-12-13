/******************************************************************************
 * Copyright (c) 2016 Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright 2016 Open Connectivity Foundation and Contributors to
 *    AllSeen Alliance. All rights reserved.
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

#import "AJSVCGenericLoggerAdapter.h"
#import "AJSVCConvertUtil.h"
#import "AJSVCGenericLoggerUtil.h"

AJSVCGenericLoggerAdapter::AJSVCGenericLoggerAdapter(id <AJSVCGenericLogger> qscGenericLogger)
{
    kAJSVCGenericLogger = qscGenericLogger;
}

AJSVCGenericLoggerAdapter::~AJSVCGenericLoggerAdapter()
{
}

void AJSVCGenericLoggerAdapter::debug(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger debugTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::info(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger infoTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::warn(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger warnTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::error(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger errorTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}

void AJSVCGenericLoggerAdapter::fatal(const char *Tag, const char *logText)
{
    [kAJSVCGenericLogger fatalTag:[AJSVCConvertUtil convertConstCharToNSString:Tag] text:[AJSVCConvertUtil convertConstCharToNSString:logText]];
}