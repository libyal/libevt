#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libevt/.libs/libevt.1.dylib" ] && [ -f ./pyevt/.libs/pyevt.so ]
then
    install_name_tool -change /usr/local/lib/libevt.1.dylib "${PWD}/libevt/.libs/libevt.1.dylib" ./pyevt/.libs/pyevt.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

