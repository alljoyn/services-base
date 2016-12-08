#!/bin/bash -xe
# #    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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
export SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}" )
. $SCRIPT_DIR/build_linux_getopts.sh

# cache core - used by build system
if [ "$SERVICE" == "core" ]; then
    if [ -z "$ALLJOYN_DIST_DIR" ]; then
        echo "Distribution directory needs to be specified to build a cached core build"
        exit 1
    fi
    ALLJOYN_DEST_DIST_DIR=$ALLJOYN_DIST_DIR
    . $SCRIPT_DIR/build_linux_core_internal.sh
    mkdir -p $ALLJOYN_DEST_DIST_DIR
    rm -rf $ALLJOYN_DEST_DIST_DIR
    cp -r $ALLJOYN_DISTDIR $ALLJOYN_DEST_DIST_DIR
    exit 0
fi

# build core
if [ -z "$ALLJOYN_DIST_DIR" ]; then
    if [ -z "$ALLJOYN_CORE_DIR" ]; then
        echo "Provide a path for either -C <alljoyn_core_dir> for source build or -D <alljoyn_distribution_dir> for an unzipped distribution"
        exit 1
    fi
    . $SCRIPT_DIR/build_linux_core_internal.sh
else
    echo "Distribution argument provided, not building from source."
    if [ ! -z "$ALLJOYN_DIST_UNZIP" ]; then
        ZIP_PATH=$ALLJOYN_DIST_DIR/$ALLJOYN_DIST_UNZIP
        NEW_DIST_PATH=$ALLJOYN_DIST_DIR/dist
        echo "Unzipping: $ZIP_PATH"
        unzip $ZIP_PATH $NEW_DIST_PATH
        export ALLJOYN_DIST_UNZIP=$NEW_DIST_PATH
    fi
fi

# build service
pushd $BUILD_ROOT/$SERVICE
scons --jobs $JOBS VARIANT=$VARIANT CPU=$CPU BINDINGS=$BINDINGS WS=$WS ALLJOYN_DISTDIR=$ALLJOYN_DIST_DIR CRYPTO=builtin
popd

# packaging
export HAS_CPP=$(echo $BINDINGS | grep -iq cpp)$?
if [ $HAS_CPP -eq 0 ]; then
    . $SCRIPT_DIR/build_linux_package_cpp.sh
fi
