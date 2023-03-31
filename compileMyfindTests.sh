#!/bin/bash
./compileMyfind.sh
gcc mainTests.c myfind.c applyTestsAndActionsTests.c commandLineParsingAndValidationTests.c iterateThroughDirectoryTreeTests.c -o myfindTests -g -Wall -Wextra -pedantic