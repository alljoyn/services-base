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
while getopts 'A:V:C:B:W:S:D:R:U:J:' opt; do
    echo "${opt} ${OPTARG}"
    case ${opt} in
        A) export ALLJOYN_CORE_DIR=${OPTARG} ;;
        D) export ALLJOYN_DIST_DIR=${OPTARG} ;;
        V) export VARIANT=${OPTARG} ;;
        C) export CPU=${OPTARG} ;;
        B) export BINDINGS=${OPTARG} ;;
        W) export WS=${OPTARG} ;;
        S) export SERVICE=${OPTARG} ;;
        R) export BUILD_ROOT=${OPTARG:-"."} ;;
        U) export ALLJOYN_DIST_UNZIP=${OPTARG} ;;
        J) export JOBS=${OPTARG} ;;
        *) echo "Invalid argument: ${OPTARG}"; #exit 1  # illegal option
    esac
done

export BUILD_ROOT=${BUILD_ROOT:-"."}
export JOBS=${JOBS:-1}

echo "Service: $SERVICE"
echo "Variant: $VARIANT"
echo "CPU: $CPU"
echo "Bindings: $BINDINGS"
echo "Whitespace: $WS"
echo "AllJoyn Core Dir: $ALLJOYN_CORE_DIR"
echo "AllJoyn Dist Dir: $ALLJOYN_DIST_DIR"
echo "AllJoyn Dist Unzip File: $ALLJOYN_DIST_UNZIP"
echo "Build Root Dir: $BUILD_ROOT"

