#!/bin/bash
# This script will compile the myfindTests program
gcc mainTests.c myfind.c applyTestsAndActionsTests.c commandLineParsingAndValidationTests.c iterateThroughDirectoryTreeTests.c -o myfindTests -g -Wall -Wextra -pedantic