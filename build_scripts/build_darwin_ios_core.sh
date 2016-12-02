#!/bin/bash -xe
# # 
#    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
#    Source Project Contributors and others.
#    
#    All rights reserved. This program and the accompanying materials are
#    made available under the terms of the Apache License, Version 2.0
#    which accompanies this distribution, and is available at
#    http://www.apache.org/licenses/LICENSE-2.0

#

VARIANT=$1

# scons call from xcodebuild hangs locally and must be built manually (i.e. open the alljoyn_darwin.xcodeproj and build each of the targets)
if [ -z $ALLJOYN_DEV ]; then
    cd core/alljoyn/alljoyn_objc
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_ios        -sdk iphoneos        -configuration $VARIANT
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_ios        -sdk iphonesimulator -configuration $VARIANT
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_ios_armv7  -sdk iphoneos        -configuration $VARIANT
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_ios_armv7s -sdk iphoneos        -configuration $VARIANT
    xcodebuild -project alljoyn_darwin.xcodeproj -scheme alljoyn_core_arm64      -sdk iphoneos        -configuration $VARIANT
    cd ../services/about/ios/samples/alljoyn_services_cpp
else
    cd $ALLJOYN_ROOT_DIR/core/alljoyn/services/about/ios/samples/alljoyn_services_cpp
fi

#$VARIANT version of alljoyn about cpp for iphoneos  
xcodebuild -project alljoyn_about_cpp.xcodeproj -scheme alljoyn_about_cpp -configuration $VARIANT -sdk iphoneos

#$VARIANT version of alljoyn about cpp for iphonesimulator
xcodebuild -project alljoyn_about_cpp.xcodeproj -scheme alljoyn_about_cpp -configuration $VARIANT -sdk iphonesimulator

cd ../alljoyn_services_objc

#$VARIANT version of alljoyn about objc for iphoneos  
xcodebuild -project alljoyn_about_objc.xcodeproj -scheme alljoyn_about_objc -configuration $VARIANT -sdk iphoneos

#$VARIANT version of alljoyn about objc for iphonesimulator
xcodebuild -project alljoyn_about_objc.xcodeproj -scheme alljoyn_about_objc -configuration $VARIANT -sdk iphonesimulator

#Library 3 = AllJoynFramework_iOS 
cd ../../../../../alljoyn_objc/AllJoynFramework_iOS

#$VARIANT version of AllJoynFramework_iOS for iphoneos
xcodebuild -project AllJoynFramework_iOS.xcodeproj -scheme AllJoynFramework_iOS -configuration $VARIANT -sdk iphoneos

#$VARIANT version of AllJoynFramework_iOS for iphonesimulator
xcodebuild      -project AllJoynFramework_iOS.xcodeproj -scheme AllJoynFramework_iOS -configuration $VARIANT -sdk iphonesimulator