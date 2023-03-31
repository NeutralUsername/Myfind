
#include "iterateThroughDirectoryTreeTests.h"
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
    char findStandardOutString[1000000] = "";
    char myfindStandardOutString[1000000] = "";
    char temp = '\0';
    while((temp = fgetc(findStandardOut)) != EOF) {
        strncat(findStandardOutString, &temp, 1);
    }
    while((temp = fgetc(myfindStandardOut)) != EOF) {
        strncat(myfindStandardOutString, &temp, 1);
    }
    for (int i = 0; i < strlen(findStandardOutString); i++) {
        if (findStandardOutString[i] == myfindStandardOutString[i]) {
            printf("%c", findStandardOutString[i]);
        } else {
            printf("error at index %d\n", i);
            return NULL;
        }
    }

    pclose(findStandardOut);
    pclose(myfindStandardOut);
    printf("iterateThroughDirectoryTree test case %s passed\n", path);
}