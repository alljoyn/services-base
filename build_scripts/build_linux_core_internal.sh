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
pushd $ALLJOYN_CORE_DIR
scons --jobs $JOBS VARIANT=$VARIANT CPU=$CPU BINDINGS=$BINDINGS WS=$WS CRYPTO=builtin
popd

export ALLJOYN_DISTDIR=$ALLJOYN_CORE_DIR/build/linux/$CPU/$VARIANT/dist