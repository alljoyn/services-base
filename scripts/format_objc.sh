#!/usr/bin/env bash

pushd $(dirname $0)/.. > /dev/null

BASE_DIR=$(pwd)
FORMAT=${BASE_DIR}/scripts/format_source.sh

for DIR in $(find . -type d -regex '.*/ios'); do
    echo "-- Checking $DIR"
    find -E $DIR -type f -regex '.*\.(h|m|mm)' -exec ${FORMAT} {} objc \;
done

popd > /dev/null
