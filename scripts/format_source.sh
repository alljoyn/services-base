#!/usr/bin/env bash

pushd $(dirname $0)/.. > /dev/null
BASE_DIR=$(pwd)
popd > /dev/null

TAG=$(basename $0)

UNCRUSTIFY=$(which uncrustify)
if [ -z "${UNCRUSTIFY}" ]; then
    echo "${TAG}: uncrustify tool missing"
    exit 1
fi

FILE=$1
if [ ! -f ${FILE} ]; then
    echo "${TAG}: input file missing (${FILE})"
    exit 2
fi

LANG=$2

UC_LANG=$3
if [ "objc" == "${UC_LANG}" ]; then
    UC_LANG_OVERRIDE="-l OC+"
else
    unset UC_LANG_OVERRIDE
fi

CONF=${BASE_DIR}/build_core/tools/conf/uncrustify_${LANG}.cfg
if [ ! -f ${CONF} ]; then
    echo "${TAG}: configuration missing (${CONF})"
    exit 3
fi

pushd $(dirname ${FILE}) > /dev/null

WORK_DIR=$(pwd)
I_NAME=$(basename ${FILE})
O_NAME=${I_NAME}.uncrustify

echo " · Formatting ${FILE}"
${UNCRUSTIFY} -q -c ${CONF} ${UC_LANG_OVERRIDE} ${I_NAME}
if [ -f ${O_NAME} ]; then
    DIFF=$(diff ${I_NAME} ${O_NAME})
    if [ ! -z "${DIFF}" ]; then
        mv -f ${O_NAME} ${I_NAME}
    else
        rm ${O_NAME}
    fi
fi

popd > /dev/null
