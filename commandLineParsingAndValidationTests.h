#ifndef COMMANDLINEPARSINGANDVALIDATIONTESTS_H
#define COMMANDLINEPARSINGANDVALIDATIONTESTS_H
#include "myfind.h"
struct commandLineParsingAndValidationTestArguments {
    int argc;
    char *argv[1000];
    Expression  expectedExpressions[123];
    int expectedExpressionCount;
    char expectedPaths[123][1000];
    int expectedPathCount;
    int testId;
};
void callCommandLineParsingAndValidationTestCases();
void *testCommandLineParsingAndValidation(void *argsTest);
#endif