#!/bin/sh

# Path for finding the test module
export LUA_CPATH=./Tests/?.so

# Runs all tests in the 'Tests' directory and displays a list with all
# failed tests. Returns 0 if all tests suceeded, 1 otherwise.


errors=""

if [ "$1" == "verbose" ]; then
    testParam="--log_level=messages"
else
    testParam=""
fi

for f in Tests/Test*.cpp; do
    testCommand=`basename $f .cpp`
    echo "Running $testCommand..."
    Tests/$testCommand $testParam
    if [ $? != 0 ]; then
        errors="$errors $testCommand"
    fi
    echo
    echo
done

echo " -----------"
echo
echo

if [ -z "$errors" ]; then
    echo "All tests suceeded."
    exit 0
else
    echo "The following tests failed: $errors"
    exit 1
fi

