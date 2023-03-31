#include "commandLineParsingAndValidationTests.h"
#include "iterateThroughDirectoryTreeTests.h"
#include "applyTestsAndActionsTests.h"
int main() {
    callCommandLineParsingAndValidationTestCases();
    callIterateThroughDirectoryTreeTestCases();
    callApplyTestsAndActionsTests();
    return 0;
}