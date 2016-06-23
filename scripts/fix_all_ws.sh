#!/usr/bin/env bash

pushd $(dirname $0)/.. > /dev/null

BASE_DIR=$(pwd)
FIX_WS=${BASE_DIR}/scripts/fix_ws.sh

function fixWsInDir {
    WORK_DIR=$1
    if [ -d $WORK_DIR ]; then
        echo " ◊ Scanning ${WORK_DIR}…"
        LANG=$2
        PATTERN=$3
        UC_LANG=$4
        find -E $WORK_DIR -type f -regex ${PATTERN} -exec ${FIX_WS} {} ${LANG} ${UC_LANG} \;
    fi
}

function fixJavaWsInDir {
    fixWsInDir $1 java '.*\.java'
}

function fixObjcWsInDir {
    fixWsInDir $1 objc '.*\.(h|m|mm)' OC+
}

for DIR in $(find . -type d -regex '.*/java/.*/src'); do
    fixJavaWsInDir $DIR
done

for DIR in $(find . -type d -regex '.*/ios'); do
    fixObjcWsInDir $DIR
done

popd > /dev/null
