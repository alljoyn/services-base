#!/usr/bin/env bash

pushd $(dirname $0)/.. > /dev/null

BASE_DIR=$(pwd)
FORMAT=${BASE_DIR}/scripts/format_source.sh

for DIR in $(find . -type d -regex '.*/cpp'); do
    echo "-- Checking $DIR"
    find -E $DIR -type f -regex '.*\.(h|c|cc)' -exec ${FORMAT} {} cpp \;
done

popd > /dev/null
