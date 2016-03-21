#!/bin/bash -xe
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
pushd $ALLJOYN_CORE_DIR
scons --jobs $JOBS VARIANT=$VARIANT CPU=$CPU BINDINGS=$BINDINGS WS=$WS CRYPTO=builtin
popd

export ALLJOYN_DISTDIR=$ALLJOYN_CORE_DIR/build/linux/$CPU/$VARIANT/dist
# hack - core/alljoyn/common/SConscript isn't installing all the qcc headers into the distributable that it should
cp -r $ALLJOYN_CORE_DIR/common/inc/qcc $ALLJOYN_DISTDIR/cpp/inc/
