#!/usr/bin/env bash

pushd $(dirname $0)/.. > /dev/null

BASE_DIR=$(pwd)
FORMAT=${BASE_DIR}/scripts/format_source.sh

for DIR in $(find . -type d -regex '.*/java/.*/src'); do
    echo "-- Checking $DIR"
    find -E $DIR -type f -regex '.*\.java' -exec ${FORMAT} {} java \;
done

popd > /dev/null
