#!/usr/bin/env bash

DIR=$(dirname $0)

$DIR/format_cpp.sh
$DIR/format_java.sh
$DIR/format_objc.sh
