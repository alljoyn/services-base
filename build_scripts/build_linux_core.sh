#!/bin/bash

export SCRIPT_DIR=$(dirname "${BASH_SOURCE[0]}" )

set -e # fail fast

. $SCRIPT_DIR/build_linux_getopts.sh

set -x # echo commands
. $SCRIPT_DIR/build_linux_core_internal.sh
