#include "myfind.h"
#include "commandLineParsingAndValidationTests.h"
#include "iterateThroughDirectoryTreeTests.h"
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


int main(int argc, char *argv[]) {
    callCommandLineParsingAndValidationTestCases();
    callIterateThroughDirectoryTreeTestCases();
    // ProcessedArguments processedArgs = commandLineParsingAndValidation(argc, argv);
    // for (int i = 0; i < processedArgs.pathCount; i++) {
    //     struct stat fileStat;
    //     if (lstat(processedArgs.paths[i], &fileStat) < 0) { //if the provided path does not exist (or cannot be accessed)
    //         printf("find: '%s': No such file or directory\n", processedArgs.paths[i]); 
    //         continue;
    //     }
    //     iterateThroughDirectoryTree(processedArgs.paths[i], processedArgs.expressions, processedArgs.expressionCount, fileStat);
    // }

    // free(processedArgs.expressions);
    // for (int i = 0; i < processedArgs.pathCount; i++) {
    //     free(processedArgs.paths[i]);
    // }
    // free(processedArgs.paths);
    return 0;
}

ProcessedArguments commandLineParsingAndValidation(int argc, char *argv[]) {
    ProcessedArguments processedArgs = {
        .expressionCount = 0,
        .expressions = NULL,
        .pathCount = 0,
        .paths = NULL
    };
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (processedArgs.expressionCount == 0) {
                processedArgs.paths = realloc(processedArgs.paths, sizeof(char*) * ((processedArgs.pathCount)+1));
                char *path = malloc(sizeof(char) * (strlen(argv[i])+1));
                strcpy(path, argv[i]);
                (processedArgs.paths)[(processedArgs.pathCount)++] = path;
            } else {
                printf("find: path must preceed expression: `%s'\n", argv[i]);
                exit(1);
            }
            continue;
        }
        ExpressionType type = getExpressionType(argv[i]);
        if (type == invalid) {
            printf("find: unknown predicate `%s'\n", argv[i]);
            exit(1);
        }
        processedArgs.expressions = realloc(processedArgs.expressions, sizeof(Expression) * ((processedArgs.expressionCount)+1));
        (processedArgs.expressions)[processedArgs.expressionCount].type = type;
        if (type == NAME || type == TYPE || type == USER) {
            if (i+1 >= argc) {
                printf("find: missing argument to `%s'\n", argv[i]);
                exit(1);
            }
            if (type == TYPE) {
                if (!isValidType(argv[i+1])) {
                    printf("find: invalid argument `%s' to `-type'\n", argv[i+1]);
                    exit(1);
                }
            }
            if (type == USER) {
                if (!isValidUser(argv[i+1])) {
                    printf("find: '%s' is not the name of a known user\n", argv[i+1]);
                    exit(1);
                }
            }
            (processedArgs.expressions)[processedArgs.expressionCount].argument = argv[i+1];
            i++;
        } else {
            (processedArgs.expressions)[processedArgs.expressionCount].argument = "";
        }
        (processedArgs.expressionCount)++;
    } 
    if (processedArgs.pathCount == 0) {
        processedArgs.pathCount = 1;
        processedArgs.paths = malloc(sizeof(char*));
        (processedArgs.paths )[0] = malloc(sizeof(char) * 2);
        (processedArgs.paths)[0][0] = '.';
        (processedArgs.paths)[0][1] = '\0';
    } 
    int printOrLsUsed = 0;
    for (int i = 0; i < (processedArgs.expressionCount); i++) {
        if ((processedArgs.expressions)[i].type == PRINT || (processedArgs.expressions)[i].type == LS) {
            printOrLsUsed = 1;
            break;
        }
    }
    if (!printOrLsUsed) {
        processedArgs.expressionCount = (processedArgs.expressionCount) + 1;
        processedArgs.expressions = realloc(processedArgs.expressions, sizeof(Expression) * (processedArgs.expressionCount));
        (processedArgs.expressions)[(processedArgs.expressionCount)-1].type = PRINT;
        (processedArgs.expressions)[(processedArgs.expressionCount)-1].argument = "";
    }
    return processedArgs;
}

void iterateThroughDirectoryTree(char *path, Expression *expressions, int expressionCount, struct stat fileStat) {
    applyTestsAndActions(expressions, expressionCount, path, fileStat);
    if (S_ISDIR(fileStat.st_mode)) {
        DIR *dir = opendir(path);
        if (dir == NULL) {
            fprintf(stderr, "find: '%s': Permission denied\n", path);
            return;
        }
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            char *newPath = malloc(strlen(path) + strlen(entry->d_name) + 2);
            strcpy(newPath, path);
            if (path[strlen(path)-1] != '/')
                strcat(newPath, "/");
            strcat(newPath, entry->d_name);
            struct stat newFileStat;
            if (lstat(newPath, &newFileStat) == 0) {
               iterateThroughDirectoryTree(newPath, expressions, expressionCount, newFileStat);
            }
            free(newPath);
        }
        closedir(dir);
    } 
}

void applyTestsAndActions(Expression *expressions, int expressionCount, char *path, struct stat fileStat) {
    for (int i = 0; i < expressionCount; i++) {
        switch (expressions[i].type) {
            case PRINT:
                printf("%s\n", path);
                break;
            case LS:
                printf("%ld\t", fileStat.st_ino);
                printf("%ld\t", fileStat.st_blocks);
                printf("%o\t", fileStat.st_mode);
                printf("%ld\t", fileStat.st_nlink);
                printf("%d\t", fileStat.st_uid);
                printf("%d\t", fileStat.st_gid);
                printf("%ld\t", fileStat.st_size);
                printf("%s\t", ctime(&fileStat.st_mtime));
                printf("%s\n", path);
                break;
            case NAME: {        
                    char *fileName = strrchr(path, '/');
                    if (fileName == NULL) {
                        fileName = path;
                    } else {
                        fileName++;
                    }
                    if (strcmp(expressions[i].argument, fileName) != 0) {
                        return;
                    }
                }
                break;
            case TYPE:
                if (strcmp(expressions[i].argument, "f") == 0) {
                    if (!S_ISREG(fileStat.st_mode)) {
                        return;
                    } 
                } else if (strcmp(expressions[i].argument, "d") == 0) {
                    if (!S_ISDIR(fileStat.st_mode)) {
                        return;
                    } 
                } else if (strcmp(expressions[i].argument, "l") == 0) {
                    if (!S_ISLNK(fileStat.st_mode)) {
                        return;
                    } 
                } else if(strcmp(expressions[i].argument, "c") == 0) {
                    if (!S_ISCHR(fileStat.st_mode)) {
                        return;
                    } 
                } else if (strcmp(expressions[i].argument, "b") == 0) {
                    if (!S_ISBLK(fileStat.st_mode)) {
                        return;
                    } 
                } else if (strcmp(expressions[i].argument, "p") == 0) {
                    if (!S_ISFIFO(fileStat.st_mode)) {
                        return;
                    } 
                } else if (strcmp(expressions[i].argument, "s") == 0) {
                    if (!S_ISSOCK(fileStat.st_mode)) {
                        return;
                    } 
                }
                break;
            case USER: {
                char *userNameOrId = expressions[i].argument;
                struct passwd *user = getpwnam(userNameOrId);
                if (user == NULL && isdigit(userNameOrId[0])) {
                    user = getpwuid(atoi(userNameOrId));
                }
                if (user->pw_uid != fileStat.st_uid) {
                    return;
                }
            }
            break;
            case invalid:
                break;
        }
    }
}

int isValidUser(char *user) {
    struct passwd *userStruct = getpwnam(user);
    if (userStruct == NULL && isdigit(user[0])) {
        userStruct = getpwuid(atoi(user));
    }
    return userStruct != NULL;
}

int isValidType(char *type) {
    if (strcmp(type, "f") == 0)
        return 1;
    if (strcmp(type, "d") == 0)
        return 1;
    if (strcmp(type, "l") == 0)
        return 1;
    if (strcmp(type, "c") == 0)
        return 1;
    if (strcmp(type, "b") == 0)
        return 1;
    if (strcmp(type, "p") == 0)
        return 1;
    if (strcmp(type, "s") == 0)
        return 1;
    return 0;
}

ExpressionType getExpressionType(char *expression) {
    if (strcmp(expression, "-print") == 0)
        return PRINT;
    if (strcmp(expression, "-ls") == 0)
        return LS;
    if (strcmp(expression, "-name") == 0)
        return NAME;
    if (strcmp(expression, "-type") == 0)
        return TYPE;
    if (strcmp(expression, "-user") == 0)
        return USER;
    return invalid;
}