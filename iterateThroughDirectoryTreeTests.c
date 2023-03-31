
#include "iterateThroughDirectoryTreeTests.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

void callIterateThroughDirectoryTreeTestCases() { 
    pthread_t thread1;  
    pthread_create(&thread1, NULL, testIterateThroughDirectoryTree, &".");
    pthread_t thread2;
    pthread_create(&thread2, NULL, testIterateThroughDirectoryTree, &"~/Downloads");
    pthread_t thread3;
    pthread_create(&thread3, NULL, testIterateThroughDirectoryTree, &"/var/cache/private");
    pthread_t thread4;
    pthread_create(&thread4, NULL, testIterateThroughDirectoryTree, &". ~/Downloads ~/myfind-bsys- /var/cache/private");
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
}

void *testIterateThroughDirectoryTree(void *voidPath) {
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
        printf("iterateThroughDirectoryTree test case %s failed\n", path);
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
    printf("iterateThroughDirectoryTree test case %s passed\n", path);
    return NULL;
}