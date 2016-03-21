#!/bin/bash
# Copyright AllSeen Alliance. All rights reserved.
#
#    Permission to use, copy, modify, and/or distribute this software for any
#    purpose with or without fee is hereby granted, provided that the above
#    copyright notice and this permission notice appear in all copies.
#
#    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
#    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
#    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
#    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
#    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
#    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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


