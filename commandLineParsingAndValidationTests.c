#include "commandLineParsingAndValidationTests.h"
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
#include <sys/types.h>
#include <grp.h>

void callCommandLineParsingAndValidationTestCases() {
    //test case 1: no arguments
    pthread_t thread1;
    struct commandLineParsingAndValidationTestArguments args1 = {
        .argc = 1,
        .argv = {
            "myfind"
        },
        .expectedExpressions = {
            {PRINT, ""}
        },
        .expectedExpressionCount = 1,
        .expectedPaths = {
            "."
        },
        .expectedPathCount = 1,
        .testId = 1
    };
    pthread_create(&thread1, NULL, testCommandLineParsingAndValidation, &args1);

    //test case 2: -name
    pthread_t thread2;
    struct commandLineParsingAndValidationTestArguments args2 = {
        .argc = 3,
        .argv = {
            "myfind",
            "-name",
            "test"
        },
        .expectedExpressions = {
            {NAME, "test"},
            {PRINT, ""}
        },
        .expectedExpressionCount = 2,
        .expectedPaths = {
            "."
        },
        .expectedPathCount = 1,
        .testId = 2
    };
    pthread_create(&thread2, NULL, testCommandLineParsingAndValidation, &args2);

    //test case 3: -type
    pthread_t thread3;
    struct commandLineParsingAndValidationTestArguments args3 = {
        .argc = 3,
        .argv = {
            "myfind",
            "-type",
            "f"
        },
        .expectedExpressions = {
            {TYPE, "f"},
            {PRINT, ""}
        },
        .expectedExpressionCount = 2,
        .expectedPaths = {
            "."
        },
        .expectedPathCount = 1,
        .testId = 3
    };
    pthread_create(&thread3, NULL, testCommandLineParsingAndValidation, &args3);

    //test case 4: -user
    pthread_t thread4;
    struct commandLineParsingAndValidationTestArguments args4 = {
        .argc = 3,
        .argv = {
            "myfind",
            "-user",
            "root"
        },
        .expectedExpressions = {
            {USER, "root"},
            {PRINT, ""}
        },
        .expectedExpressionCount = 2,
        .expectedPaths = {
            "."
        },
        .expectedPathCount = 1,
        .testId = 4
    };
    pthread_create(&thread4, NULL, testCommandLineParsingAndValidation, &args4);

    //test case 5: -print
    pthread_t thread5;
    struct commandLineParsingAndValidationTestArguments args5 = {
        .argc = 2,
        .argv = {
            "myfind",
            "-print"
        },
        .expectedExpressions = {
            {PRINT, ""}
        },
        .expectedExpressionCount = 1,
        .expectedPaths = {
            "."
        },
        .expectedPathCount = 1,
        .testId = 5
    };
    pthread_create(&thread5, NULL, testCommandLineParsingAndValidation, &args5);

    //test case 6: -ls
    pthread_t thread6;
    struct commandLineParsingAndValidationTestArguments args6 = {
        .argc = 2,
        .argv = {
            "myfind",
            "-ls"
        },
        .expectedExpressions = {
            {LS, ""}
        },
        .expectedExpressionCount = 1,
        .expectedPaths = {
            "."
        },
        .expectedPathCount = 1,
        .testId = 6
    };
    pthread_create(&thread6, NULL, testCommandLineParsingAndValidation, &args6);

    //test case 7: one path no expressions
    pthread_t thread7;
    struct commandLineParsingAndValidationTestArguments args7 = {
        .argc = 2,
        .argv = {
            "myfind",
            "/home"
        },
        .expectedExpressions = {
            {PRINT, ""}
        },
        .expectedExpressionCount = 1,
        .expectedPaths = {
            "/home"
        },
        .expectedPathCount = 1,
        .testId = 7
    };
    pthread_create(&thread7, NULL, testCommandLineParsingAndValidation, &args7);

    //test case 8: one path one expression (ls)
    pthread_t thread8;
    struct commandLineParsingAndValidationTestArguments args8 =  {
        .argc = 3,
        .argv = {
            "myfind",
            "/home",
            "-ls"
        },
        .expectedExpressions = {
            {LS, ""}
        },
        .expectedExpressionCount = 1,
        .expectedPaths = {
            "/home"
        },
        .expectedPathCount = 1,
        .testId = 8
    };
    pthread_create(&thread8, NULL, testCommandLineParsingAndValidation, &args8);
    
    //test case 9: two paths no expressions
    pthread_t thread9;
    struct commandLineParsingAndValidationTestArguments args9 = {
        .argc = 3,
        .argv = {
            "myfind",
            "/home",
            "/etc"
        },
        .expectedExpressions = {
            {PRINT, ""}
        },
        .expectedExpressionCount = 1,
        .expectedPaths = {
            "/home",
            "/etc"
        },
        .expectedPathCount = 2,
        .testId = 9
    };
    pthread_create(&thread9, NULL, testCommandLineParsingAndValidation, &args9);

    //test case 10: two paths one expression (name)
    pthread_t thread10;
    struct commandLineParsingAndValidationTestArguments args10 = {
        .argc = 5,
        .argv = {
            "myfind",
            "/home",
            "/etc",
            "-name",
            "test"
        },
        .expectedExpressions = {
            {NAME, "test"},
            {PRINT, ""}
        },
        .expectedExpressionCount = 2,
        .expectedPaths = {
            "/home",
            "/etc"
        },
        .expectedPathCount = 2,
        .testId = 10
    };
    pthread_create(&thread10, NULL, testCommandLineParsingAndValidation, &args10);

    //test case 11: two paths two expressions (ls and print)
    pthread_t thread11;
    struct commandLineParsingAndValidationTestArguments args11 = {
        .argc = 5,
        .argv = {
            "myfind",
            "/home",
            "/etc",
            "-ls",
            "-print"
        },
        .expectedExpressions = {
            {LS, ""},
            {PRINT, ""}
        },
        .expectedExpressionCount = 2,
        .expectedPaths = {
            "/home",
            "/etc"
        },
        .expectedPathCount = 2,
        .testId = 11
    };
    pthread_create(&thread11, NULL, testCommandLineParsingAndValidation, &args11);

    //test case 12: two paths three expressions (ls, print, name)    
    pthread_t thread12;
    struct commandLineParsingAndValidationTestArguments args12 = {
        .argc = 7,
        .argv = {
            "myfind",
            "/home",
            "/etc",
            "-ls",
            "-print",
            "-name",
            "test"
        },
        .expectedExpressions = {
            {LS, ""},
            {PRINT, ""},
            {NAME, "test"}
        },
        .expectedExpressionCount = 3,
        .expectedPaths = {
            "/home",
            "/etc"
        },
        .expectedPathCount = 2,
        .testId = 12
    };
    pthread_create(&thread12, NULL, testCommandLineParsingAndValidation, &args12);

    //test case 13: two paths three expressions (type, user, ls)
    pthread_t thread13;
    struct commandLineParsingAndValidationTestArguments args13 = {
        .argc = 8,
        .argv = {
            "myfind",
            "/home",
            "/etc",
            "-type",
            "f",
            "-user",
            "root",
            "-ls"
        },
        .expectedExpressions = {
            {TYPE, "f"},
            {USER, "root"},
            {LS, ""}
        },
        .expectedExpressionCount = 3,
        .expectedPaths = {
            "/home",
            "/etc"
        },
        .expectedPathCount = 2,
        .testId = 13
    };
    pthread_create(&thread13, NULL, testCommandLineParsingAndValidation, &args13);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
    pthread_join(thread7, NULL);
    pthread_join(thread8, NULL);
    pthread_join(thread9, NULL);
    pthread_join(thread10, NULL);
    pthread_join(thread11, NULL);
    pthread_join(thread12, NULL);
    pthread_join(thread13, NULL);
}

void* testCommandLineParsingAndValidation(void *voidArgs) {
    struct commandLineParsingAndValidationTestArguments *properlyTypedArgs = (struct commandLineParsingAndValidationTestArguments*)voidArgs;
    ProcessedArguments processedArgs = commandLineParsingAndValidation(properlyTypedArgs->argc, properlyTypedArgs->argv);
    if (processedArgs.expressionCount != properlyTypedArgs->expectedExpressionCount) {
        printf("testCommandLineParsingAndValidation: expressionCount is %d, expected %d ... in test %d\n", processedArgs.expressionCount, properlyTypedArgs->expectedExpressionCount, properlyTypedArgs->testId);
        free(processedArgs.expressions);
        for (int i = 0; i < processedArgs.pathCount; i++) {
            free(processedArgs.paths[i]);
        }
        free(processedArgs.paths);
        return NULL;
    }
    for (int i = 0; i < processedArgs.expressionCount; i++) {
        if (processedArgs.expressions[i].type != properlyTypedArgs->expectedExpressions[i].type) {
            printf("testCommandLineParsingAndValidation: expression type is %d, expected %d ... in test %d\n", processedArgs.expressions[i].type, properlyTypedArgs->expectedExpressions[i].type, properlyTypedArgs->testId);
            free(processedArgs.expressions);
            for (int i = 0; i < processedArgs.pathCount; i++) {
                free(processedArgs.paths[i]);
            }
            free(processedArgs.paths);
            return NULL;
        }
        if (processedArgs.expressions[i].argument != NULL && properlyTypedArgs->expectedExpressions[i].argument != NULL) {
            if (strcmp(processedArgs.expressions[i].argument, properlyTypedArgs->expectedExpressions[i].argument) != 0) {
                printf("testCommandLineParsingAndValidation: expression argument is %s, expected %s ... in test %d\n", processedArgs.expressions[i].argument, properlyTypedArgs->expectedExpressions[i].argument, properlyTypedArgs->testId);
                free(processedArgs.expressions);
                for (int i = 0; i < processedArgs.pathCount; i++) {
                    free(processedArgs.paths[i]);
                }
                free(processedArgs.paths);
                return NULL;
            }
        }
        if (processedArgs.expressions[i].argument == NULL && properlyTypedArgs->expectedExpressions[i].argument != NULL) {
            printf("testCommandLineParsingAndValidation: expression argument is NULL, expected %s ... in test %d\n", properlyTypedArgs->expectedExpressions[i].argument, properlyTypedArgs->testId);
            free(processedArgs.expressions);
            for (int i = 0; i < processedArgs.pathCount; i++) {
                free(processedArgs.paths[i]);
            }
            free(processedArgs.paths);
            return NULL;
        }
        if (processedArgs.expressions[i].argument != NULL && properlyTypedArgs->expectedExpressions[i].argument == NULL) {
            printf("testCommandLineParsingAndValidation: expression argument is %s, expected NULL ... in test %d\n", processedArgs.expressions[i].argument, properlyTypedArgs->testId);
            free(processedArgs.expressions);
            for (int i = 0; i < processedArgs.pathCount; i++) {
                free(processedArgs.paths[i]);
            }
            free(processedArgs.paths);
            return NULL;
        } 
    }
    if (processedArgs.pathCount != properlyTypedArgs->expectedPathCount) {
        printf("testCommandLineParsingAndValidation: pathCount is %d, expected %d ... in test %d\n", processedArgs.pathCount, properlyTypedArgs->expectedPathCount, properlyTypedArgs->testId);
        free(processedArgs.expressions);
        for (int i = 0; i < processedArgs.pathCount; i++) {
            free(processedArgs.paths[i]);
        }
        free(processedArgs.paths);
        return NULL;
    }
    for (int i = 0; i < processedArgs.pathCount; i++) {
        if (strcmp(processedArgs.paths[i], properlyTypedArgs->expectedPaths[i]) != 0) {
            printf("testCommandLineParsingAndValidation: path is %s, expected %s ... in test %d\n", processedArgs.paths[i], properlyTypedArgs->expectedPaths[i], properlyTypedArgs->testId);
            free(processedArgs.expressions);
            for (int i = 0; i < processedArgs.pathCount; i++) {
                free(processedArgs.paths[i]);
            }
            free(processedArgs.paths);
            return NULL;
        }
    }
    printf("testCommandLineParsingAndValidation: test %d passed\n", properlyTypedArgs->testId);
    free(processedArgs.expressions);
    for (int i = 0; i < processedArgs.pathCount; i++) {
        free(processedArgs.paths[i]);
    }
    free(processedArgs.paths);
    return NULL;
}