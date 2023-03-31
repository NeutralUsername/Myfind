
#include "applyTestsAndActionsTests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <ctype.h>
#include <sys/types.h>
#include <grp.h>
#include <pthread.h>

void callApplyTestsAndActionsTests() { 
    // test case 1 -print
    pthread_t thread1;  
    pthread_create(&thread1, NULL, testApplyTestsAndActions, &"~ -print");

    // test case 2 -ls
    pthread_t thread2;
    pthread_create(&thread2, NULL, testApplyTestsAndActions, &"~ -ls");

    // test case 3 -name temp
    pthread_t thread3;
    pthread_create(&thread3, NULL, testApplyTestsAndActions, &"~ -name temp");

    // test case 4 -user root
    pthread_t thread4;
    pthread_create(&thread4, NULL, testApplyTestsAndActions, &"~ -user root");

    // test case 5 -type b
    pthread_t thread5;
    pthread_create(&thread5, NULL, testApplyTestsAndActions, &"~ -type b");

    // test case 6 -type c
    pthread_t thread6;
    pthread_create(&thread6, NULL, testApplyTestsAndActions, &"~ -type c");

    // test case 7 -type d
    pthread_t thread7;
    pthread_create(&thread7, NULL, testApplyTestsAndActions, &"~ -type d");

    // test case 8 -type p
    pthread_t thread8;
    pthread_create(&thread8, NULL, testApplyTestsAndActions, &"~ -type p");

    // test case 9 -type f
    pthread_t thread9;
    pthread_create(&thread9, NULL, testApplyTestsAndActions, &"~ -type f");

    // test case 10 -type l
    pthread_t thread10;
    pthread_create(&thread10, NULL, testApplyTestsAndActions, &"~ -type l");

    // test case 11 -type s
    pthread_t thread11;
    pthread_create(&thread11, NULL, testApplyTestsAndActions, &"~ -type s");

    // test case 12 -type f -name temp
    pthread_t thread12;
    pthread_create(&thread12, NULL, testApplyTestsAndActions, &"~ -type f -name temp");

    // test case 13 path print -type f -ls
    pthread_t thread13;
    pthread_create(&thread13, NULL, testApplyTestsAndActions, &"~ -print -type f -ls");
    
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
}

void *testApplyTestsAndActions(void *voidPath) {
    char *path = (char*)voidPath;
    char findString[1000] = "find ";
    char myfindString[1000] = "./myfind ";
    strcat(findString, path);
    strcat(myfindString, path);
    FILE *findStandardOut = popen(findString, "r");
    FILE *myfindStandardOut = popen(myfindString, "r");
    char *findStandardOutString = malloc(1);
    char *myfindStandardOutString = malloc(1);
    findStandardOutString[0] = '\0';
    myfindStandardOutString[0] = '\0';
    char temp = '\0';
    int i = 0;
    int linesMyfind = 0;
    int linesFind = 0;
    while((temp = fgetc(findStandardOut)) != EOF) {
        findStandardOutString[i] = temp;
        if (temp == '\n') {
            linesFind++;
        }
        i++;
        findStandardOutString = realloc(findStandardOutString, i+1);
        findStandardOutString[i] = '\0';
    }
    i = 0;
    while((temp = fgetc(myfindStandardOut)) != EOF) {
        myfindStandardOutString[i] = temp;
        if (temp == '\n') {
            linesMyfind++;
        }
        i++;
        myfindStandardOutString = realloc(myfindStandardOutString, i+1);
        myfindStandardOutString[i] = '\0';
    }
    if(linesMyfind != linesFind) {
        printf("applyTestsAndActions test case %s failed\n", path);
        printf("myfind found %d lines, find found %d lines\n", linesMyfind, linesFind);
        printf("myfind output:\n%s\n", myfindStandardOutString);
        printf("find output:\n%s\n", findStandardOutString);
        pclose(findStandardOut);
        pclose(myfindStandardOut);
        free(findStandardOutString);
        free(myfindStandardOutString);
        return NULL;
    }
    pclose(findStandardOut);
    pclose(myfindStandardOut);
    free(findStandardOutString);
    free(myfindStandardOutString);
    printf("applyTestsAndActions test case %s passed\n", path);
    return NULL;
}