#include "tests.h"
#include "myfind.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <ctype.h>


void callCommandLineParsingAndValidationTestCases() {
    //test case 1: no arguments
    pthread_t thread1;
    struct commandLineParsingAndValidationTestArguments *args1 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args1->argc = 1;
    args1->argv = malloc(sizeof(char*));
    args1->argv[0] = "myfind";
    args1->pExpectedExpressions = malloc(sizeof(Expression));
    args1->pExpectedExpressions[0].type = PRINT;
    args1->pExpectedExpressions[0].argument = "";
    args1->expectedExpressionCount = 1;
    args1->pExpectedPaths = malloc(sizeof(char*));
    args1->pExpectedPaths[0] = ".";
    args1->expectedPathCount = 1;
    args1->testId = 1;
    pthread_create(&thread1, NULL, testCommandLineParsingAndValidation, args1);
    pthread_join(thread1, NULL);    
    freeArgs(args1);

    //test case 2: -name
    pthread_t thread2;
    struct commandLineParsingAndValidationTestArguments *args2 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args2->argc = 3;
    args2->argv = malloc(sizeof(char*) * 3);
    args2->argv[0] = "myfind";
    args2->argv[1] = "-name";
    args2->argv[2] = "test";
    args2->pExpectedExpressions = malloc(sizeof(Expression) * 2);
    args2->pExpectedExpressions[0].type = NAME;
    args2->pExpectedExpressions[0].argument = "test";
    args2->pExpectedExpressions[1].type = PRINT;
    args2->pExpectedExpressions[1].argument = "";
    args2->expectedExpressionCount = 2;
    args2->pExpectedPaths = malloc(sizeof(char*));
    args2->pExpectedPaths[0] = ".";
    args2->expectedPathCount = 1;
    args2->testId = 2;
    pthread_create(&thread2, NULL, testCommandLineParsingAndValidation, args2);
    pthread_join(thread2, NULL);
    freeArgs(args2);

    //test case 3: -type
    pthread_t thread3;
    struct commandLineParsingAndValidationTestArguments *args3 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args3->argc = 3;
    args3->argv = malloc(sizeof(char*) * 3);
    args3->argv[0] = "myfind";
    args3->argv[1] = "-type";
    args3->argv[2] = "f";
    args3->pExpectedExpressions = malloc(sizeof(Expression) * 2);
    args3->pExpectedExpressions[0].type = TYPE;
    args3->pExpectedExpressions[0].argument = "f";
    args3->pExpectedExpressions[1].type = PRINT;
    args3->pExpectedExpressions[1].argument = "";
    args3->expectedExpressionCount = 2;
    args3->pExpectedPaths = malloc(sizeof(char*));
    args3->pExpectedPaths[0] = ".";
    args3->expectedPathCount = 1;
    args3->testId = 3;
    pthread_create(&thread3, NULL, testCommandLineParsingAndValidation, args3);
    pthread_join(thread3, NULL);
    freeArgs(args3);

    //test case 4: -user
    pthread_t thread4;
    struct commandLineParsingAndValidationTestArguments *args4 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args4->argc = 3;
    args4->argv = malloc(sizeof(char*) * 3);
    args4->argv[0] = "myfind";
    args4->argv[1] = "-user";
    args4->argv[2] = "root";
    args4->pExpectedExpressions = malloc(sizeof(Expression) * 2);
    args4->pExpectedExpressions[0].type = USER;
    args4->pExpectedExpressions[0].argument = "root";
    args4->pExpectedExpressions[1].type = PRINT;
    args4->pExpectedExpressions[1].argument = "";
    args4->expectedExpressionCount = 2;
    args4->pExpectedPaths = malloc(sizeof(char*));
    args4->pExpectedPaths[0] = ".";
    args4->expectedPathCount = 1;
    args4->testId = 4;
    pthread_create(&thread4, NULL, testCommandLineParsingAndValidation, args4);
    pthread_join(thread4, NULL);
    freeArgs(args4);

    //test case 5: -print
    pthread_t thread5;
    struct commandLineParsingAndValidationTestArguments *args5 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args5->argc = 2;
    args5->argv = malloc(sizeof(char*) * 2);
    args5->argv[0] = "myfind";
    args5->argv[1] = "-print";
    args5->pExpectedExpressions = malloc(sizeof(Expression));
    args5->pExpectedExpressions[0].type = PRINT;
    args5->pExpectedExpressions[0].argument = "";
    args5->expectedExpressionCount = 1;
    args5->pExpectedPaths = malloc(sizeof(char*));
    args5->pExpectedPaths[0] = ".";
    args5->expectedPathCount = 1;
    args5->testId = 5;
    pthread_create(&thread5, NULL, testCommandLineParsingAndValidation, args5);
    pthread_join(thread5, NULL);
    freeArgs(args5);

    //test case 6: -ls
    pthread_t thread6;
    struct commandLineParsingAndValidationTestArguments *args6 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args6->argc = 2;
    args6->argv = malloc(sizeof(char*) * 2);
    args6->argv[0] = "myfind";
    args6->argv[1] = "-ls";
    args6->pExpectedExpressions = malloc(sizeof(Expression));
    args6->pExpectedExpressions[0].type = LS;
    args6->pExpectedExpressions[0].argument = "";
    args6->expectedExpressionCount = 1;
    args6->pExpectedPaths = malloc(sizeof(char*));
    args6->pExpectedPaths[0] = ".";
    args6->expectedPathCount = 1;
    args6->testId = 6;
    pthread_create(&thread6, NULL, testCommandLineParsingAndValidation, args6);
    pthread_join(thread6, NULL);
    freeArgs(args6);

    //test case 7: one path no expressions
    pthread_t thread7;
    struct commandLineParsingAndValidationTestArguments *args7 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args7->argc = 2;
    args7->argv = malloc(sizeof(char*) * 2);
    args7->argv[0] = "myfind";
    args7->argv[1] = "/home";
    args7->pExpectedExpressions = malloc(sizeof(Expression));
    args7->pExpectedExpressions[0].type = PRINT;
    args7->pExpectedExpressions[0].argument = "";
    args7->expectedExpressionCount = 1;
    args7->pExpectedPaths = malloc(sizeof(char*));
    args7->pExpectedPaths[0] = "/home";
    args7->expectedPathCount = 1;
    args7->testId = 7;
    pthread_create(&thread7, NULL, testCommandLineParsingAndValidation, args7);
    pthread_join(thread7, NULL);
    freeArgs(args7);

    //test case 8: one path one expression (ls)
    pthread_t thread8;
    struct commandLineParsingAndValidationTestArguments *args9 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args9->argc = 3;
    args9->argv = malloc(sizeof(char*) * 3);
    args9->argv[0] = "myfind";
    args9->argv[1] = "/home";
    args9->argv[2] = "-ls";
    args9->pExpectedExpressions = malloc(sizeof(Expression));
    args9->pExpectedExpressions[0].type = LS;
    args9->pExpectedExpressions[0].argument = "";
    args9->expectedExpressionCount = 1;
    args9->pExpectedPaths = malloc(sizeof(char*));
    args9->pExpectedPaths[0] = "/home";
    args9->expectedPathCount = 1;
    args9->testId = 8;
    pthread_create(&thread8, NULL, testCommandLineParsingAndValidation, args9);
    pthread_join(thread8, NULL);
    freeArgs(args9);
    
    //test case 9: two paths no expressions
    pthread_t thread9;
    struct commandLineParsingAndValidationTestArguments *args8 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args8->argc = 3;
    args8->argv = malloc(sizeof(char*) * 3);
    args8->argv[0] = "myfind";
    args8->argv[1] = "/home";
    args8->argv[2] = "/etc";
    args8->pExpectedExpressions = malloc(sizeof(Expression));
    args8->pExpectedExpressions[0].type = PRINT;
    args8->pExpectedExpressions[0].argument = "";
    args8->expectedExpressionCount = 1;
    args8->pExpectedPaths = malloc(sizeof(char*) * 2);
    args8->pExpectedPaths[0] = "/home";
    args8->pExpectedPaths[1] = "/etc";
    args8->expectedPathCount = 2;
    args8->testId = 9;
    pthread_create(&thread9, NULL, testCommandLineParsingAndValidation, args8);
    pthread_join(thread9, NULL);
    freeArgs(args8);

    //test case 10: two paths one expression (ls)
    pthread_t thread10;
    struct commandLineParsingAndValidationTestArguments *args10 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args10->argc = 4;
    args10->argv = malloc(sizeof(char*) * 4);
    args10->argv[0] = "myfind";
    args10->argv[1] = "/home";
    args10->argv[2] = "/etc";
    args10->argv[3] = "-ls";
    args10->pExpectedExpressions = malloc(sizeof(Expression));
    args10->pExpectedExpressions[0].type = LS;
    args10->pExpectedExpressions[0].argument = "";
    args10->expectedExpressionCount = 1;
    args10->pExpectedPaths = malloc(sizeof(char*) * 2);
    args10->pExpectedPaths[0] = "/home";
    args10->pExpectedPaths[1] = "/etc";
    args10->expectedPathCount = 2;
    args10->testId = 10;
    pthread_create(&thread10, NULL, testCommandLineParsingAndValidation, args10);
    pthread_join(thread10, NULL);
    freeArgs(args10);

    //test case 11: two paths two expressions (ls and print)
    pthread_t thread11;
    struct commandLineParsingAndValidationTestArguments *args11 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args11->argc = 5;
    args11->argv = malloc(sizeof(char*) * 5);
    args11->argv[0] = "myfind";
    args11->argv[1] = "/home";
    args11->argv[2] = "/etc";
    args11->argv[3] = "-ls";
    args11->argv[4] = "-print";
    args11->pExpectedExpressions = malloc(sizeof(Expression) * 2);
    args11->pExpectedExpressions[0].type = LS;
    args11->pExpectedExpressions[0].argument = "";
    args11->pExpectedExpressions[1].type = PRINT;
    args11->pExpectedExpressions[1].argument = "";
    args11->expectedExpressionCount = 2;
    args11->pExpectedPaths = malloc(sizeof(char*) * 2);
    args11->pExpectedPaths[0] = "/home";
    args11->pExpectedPaths[1] = "/etc";
    args11->expectedPathCount = 2;
    args11->testId = 11;
    pthread_create(&thread11, NULL, testCommandLineParsingAndValidation, args11);
    pthread_join(thread11, NULL);
    freeArgs(args11);

    //test case 12: two paths three expressions (ls, print, name)    
    pthread_t thread12;
    struct commandLineParsingAndValidationTestArguments *args12 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args12->argc = 7;
    args12->argv = malloc(sizeof(char*) * 7);
    args12->argv[0] = "myfind";
    args12->argv[1] = "/home";
    args12->argv[2] = "/etc";
    args12->argv[3] = "-ls";
    args12->argv[4] = "-print";
    args12->argv[5] = "-name";
    args12->argv[6] = "a.out";
    args12->pExpectedExpressions = malloc(sizeof(Expression) * 3);
    args12->pExpectedExpressions[0].type = LS;
    args12->pExpectedExpressions[0].argument = "";
    args12->pExpectedExpressions[1].type = PRINT;
    args12->pExpectedExpressions[1].argument = "";
    args12->pExpectedExpressions[2].type = NAME;
    args12->pExpectedExpressions[2].argument = "a.out";
    args12->expectedExpressionCount = 3;
    args12->pExpectedPaths = malloc(sizeof(char*) * 2);
    args12->pExpectedPaths[0] = "/home";
    args12->pExpectedPaths[1] = "/etc";
    args12->expectedPathCount = 2;
    args12->testId = 12;
    pthread_create(&thread12, NULL, testCommandLineParsingAndValidation, args12);
    pthread_join(thread12, NULL);
    freeArgs(args12);

    //test case 13: two paths three expressions (ls, user, print)
    pthread_t thread13;
    struct commandLineParsingAndValidationTestArguments *args13 = malloc(sizeof(struct commandLineParsingAndValidationTestArguments));
    args13->argc = 7;
    args13->argv = malloc(sizeof(char*) * 7);
    args13->argv[0] = "myfind";
    args13->argv[1] = "/home";
    args13->argv[2] = "/etc";
    args13->argv[3] = "-ls";
    args13->argv[4] = "-user";
    args13->argv[5] = "root";
    args13->argv[6] = "-print";
    args13->pExpectedExpressions = malloc(sizeof(Expression) * 3);
    args13->pExpectedExpressions[0].type = LS;
    args13->pExpectedExpressions[0].argument = "";
    args13->pExpectedExpressions[1].type = USER;
    args13->pExpectedExpressions[1].argument = "root";
    args13->pExpectedExpressions[2].type = PRINT;
    args13->pExpectedExpressions[2].argument = "";
    args13->expectedExpressionCount = 3;
    args13->pExpectedPaths = malloc(sizeof(char*) * 2);
    args13->pExpectedPaths[0] = "/home";
    args13->pExpectedPaths[1] = "/etc";
    args13->expectedPathCount = 2;
    args13->testId = 13;
    pthread_create(&thread13, NULL, testCommandLineParsingAndValidation, args13);
    pthread_join(thread13, NULL);
    freeArgs(args13);
}

void* testCommandLineParsingAndValidation(void *voidArgs) {
    struct commandLineParsingAndValidationTestArguments *properlyTypedArgs = (struct commandLineParsingAndValidationTestArguments*)voidArgs;
    int expressionCount = 0;
    Expression *expressions = NULL;
    int pathCount = 0;
    char **paths = NULL;
    commandLineParsingAndValidation(properlyTypedArgs->argc, properlyTypedArgs->argv, &expressions, &expressionCount, &paths, &pathCount);
    if (expressionCount != properlyTypedArgs->expectedExpressionCount) {
        printf("testCommandLineParsingAndValidation: expressionCount is %d, expected %d ... in test %d\n", expressionCount, properlyTypedArgs->expectedExpressionCount, properlyTypedArgs->testId);
        exit(1);
    }
    for (int i = 0; i < expressionCount; i++) {
        if (expressions[i].type != properlyTypedArgs->pExpectedExpressions[i].type) {
            printf("testCommandLineParsingAndValidation: expression type is %d, expected %d ... in test %d\n", expressions[i].type, properlyTypedArgs->pExpectedExpressions[i].type, properlyTypedArgs->testId);
            exit(1);
        }
        if (expressions[i].argument != NULL && properlyTypedArgs->pExpectedExpressions[i].argument != NULL) {
            if (strcmp(expressions[i].argument, properlyTypedArgs->pExpectedExpressions[i].argument) != 0) {
                printf("testCommandLineParsingAndValidation: expression argument is %s, expected %s ... in test %d\n", expressions[i].argument, properlyTypedArgs->pExpectedExpressions[i].argument, properlyTypedArgs->testId);
                exit(1);
            }
        }
        if (expressions[i].argument == NULL && properlyTypedArgs->pExpectedExpressions[i].argument != NULL) {
            printf("testCommandLineParsingAndValidation: expression argument is NULL, expected %s ... in test %d\n", properlyTypedArgs->pExpectedExpressions[i].argument, properlyTypedArgs->testId);
            exit(1);
        }
        if (expressions[i].argument != NULL && properlyTypedArgs->pExpectedExpressions[i].argument == NULL) {
            printf("testCommandLineParsingAndValidation: expression argument is %s, expected NULL ... in test %d\n", expressions[i].argument, properlyTypedArgs->testId);
            exit(1);
        } 
    }
    if (pathCount != properlyTypedArgs->expectedPathCount) {
        printf("testCommandLineParsingAndValidation: pathCount is %d, expected %d ... in test %d\n", pathCount, properlyTypedArgs->expectedPathCount, properlyTypedArgs->testId);
        exit(1);
    }
    for (int i = 0; i < pathCount; i++) {
        if (strcmp(paths[i], properlyTypedArgs->pExpectedPaths[i]) != 0) {
            printf("testCommandLineParsingAndValidation: path is %s, expected %s ... in test %d\n", paths[i], properlyTypedArgs->pExpectedPaths[i], properlyTypedArgs->testId);
            exit(1);
        }
    }
    printf("testCommandLineParsingAndValidation: test %d passed\n", properlyTypedArgs->testId);
    free(expressions);
    for (int i = 0; i < pathCount; i++) {
        free(paths[i]);
    }
    free(paths);
    return NULL;
}

void freeArgs(struct commandLineParsingAndValidationTestArguments *args) {
    free(args->argv);
    free(args->pExpectedExpressions);
    free(args->pExpectedPaths);
    free(args);
}
