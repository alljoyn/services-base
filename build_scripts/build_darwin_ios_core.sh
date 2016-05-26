#!/bin/bash -xe
# # Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
#    Source Project (AJOSP) Contributors and others.
#
#    SPDX-License-Identifier: Apache-2.0
#
#    All rights reserved. This program and the accompanying materials are
#    made available under the terms of the Apache License, Version 2.0
#    which accompanies this distribution, and is available at
#    http://www.apache.org/licenses/LICENSE-2.0
#
#    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
#    Alliance. All rights reserved.
#
#    Permission to use, copy, modify, and/or distribute this software for
#    any purpose with or without fee is hereby granted, provided that the
#    above copyright notice and this permission notice appear in all
#    copies.
#
#     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
#     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
#     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
#     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
#     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
#     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#     PERFORMANCE OF THIS SOFTWARE.
#

VARIANT=$1

# scons call from xcodebuild hangs locally and must be built manually (i.e. open the alljoyn_darwin.xcodeproj and build each of the targets)
if [ -z $ALLJOYN_DEV ]; then
    cd core/alljoyn/alljoyn_objc
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_ios        -sdk iphoneos        -configuration $VARIANT PLATFORM_NAME=iphoneos   
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_ios        -sdk iphonesimulator -configuration $VARIANT PLATFORM_NAME=iphonesimulator
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_ios_armv7  -sdk iphoneos        -configuration $VARIANT PLATFORM_NAME=iphoneos 
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_ios_armv7s -sdk iphoneos        -configuration $VARIANT PLATFORM_NAME=iphoneos 
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_arm64      -sdk iphoneos        -configuration $VARIANT PLATFORM_NAME=iphoneos
    cd ../services/about/ios/samples/alljoyn_services_cpp
else
    cd $ALLJOYN_ROOT_DIR/core/alljoyn/services/about/ios/samples/alljoyn_services_cpp
fi

#$VARIANT version of alljoyn about cpp for iphoneos  
xcodebuild -project alljoyn_about_cpp.xcodeproj -scheme alljoyn_about_cpp ONLY_ACTIVE_ARCHS=NO -configuration $VARIANT -sdk iphoneos 

#$VARIANT version of alljoyn about cpp for iphonesimulator
xcodebuild -project alljoyn_about_cpp.xcodeproj -scheme alljoyn_about_cpp ONLY_ACTIVE_ARCHS=NO -configuration $VARIANT -sdk iphonesimulator VALID_ARCHS="i386 x86_64" PLATFORM_NAME=iphonesimulator

cd ../alljoyn_services_objc

#$VARIANT version of alljoyn about objc for iphoneos  
xcodebuild -project alljoyn_about_objc.xcodeproj -scheme alljoyn_about_objc ONLY_ACTIVE_ARCHS=NO -configuration $VARIANT -sdk iphoneos 

#$VARIANT version of alljoyn about objc for iphonesimulator
xcodebuild -project alljoyn_about_objc.xcodeproj -scheme alljoyn_about_objc ONLY_ACTIVE_ARCHS=NO -configuration $VARIANT -sdk iphonesimulator VALID_ARCHS="i386 x86_64" PLATFORM_NAME=iphonesimulator

#Library 3 = AllJoynFramework_iOS 
cd ../../../../../alljoyn_objc/AllJoynFramework_iOS

#$VARIANT version of AllJoynFramework_iOS for iphoneos
xcodebuild -project AllJoynFramework_iOS.xcodeproj -scheme AllJoynFramework_iOS ONLY_ACTIVE_ARCHS=NO -configuration $VARIANT -sdk iphoneos 

#$VARIANT version of AllJoynFramework_iOS for iphonesimulator
xcodebuild      -project AllJoynFramework_iOS.xcodeproj -scheme AllJoynFramework_iOS ONLY_ACTIVE_ARCHS=NO -configuration $VARIANT -sdk iphonesimulator VALID_ARCHS="i386 x86_64" PLATFORM_NAME=iphonesimulator