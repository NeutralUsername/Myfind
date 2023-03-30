#ifndef TESTS_H
#define TESTS_H
#include "myfind.h"
struct commandLineParsingAndValidationTestArguments {
    int argc;
    char **argv;
    Expression *pExpectedExpressions;
    int expectedExpressionCount;
    char **pExpectedPaths;
    int expectedPathCount;
    int testId;
};
void callCommandLineParsingAndValidationTestCases();
void* testCommandLineParsingAndValidation(void *argsTest);
void freeArgs(struct commandLineParsingAndValidationTestArguments *args);
#endif