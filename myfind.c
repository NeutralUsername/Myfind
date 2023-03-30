#include "myfind.h"
#include "tests.h"
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

int main(int argc, char *argv[]) {
    callCommandLineParsingAndValidationTestCases();
    int expressionCount = 0;
    Expression *expressions = NULL;
    int pathCount = 0;
    char **paths = NULL;
    commandLineParsingAndValidation(argc, argv, &expressions, &expressionCount, &paths, &pathCount);
    for (int i = 0; i < pathCount; i++) {
        struct stat fileStat;
        if (stat(paths[i], &fileStat) < 0) { //if the provided path does not exist (or cannot be accessed)
            printf("find: '%s': No such file or directory\n", paths[i]); 
            continue;
        }
        iterateThroughDirectoryTree(paths[i], expressions, expressionCount, fileStat);
    }
    free(expressions);
    for (int i = 0; i < pathCount; i++) {
        free(paths[i]);
    }
    free(paths);
    return 0;
}


void commandLineParsingAndValidation(int argc, char *argv[], Expression **pExpressions, int *pExpressionCount, char ***pPaths, int *pPathCount) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (*pExpressionCount == 0) {
                *pPaths = realloc(*pPaths, sizeof(char*) * ((*pPathCount)+1));
                char *path = malloc(sizeof(char) * (strlen(argv[i])+1));
                strcpy(path, argv[i]);
                (*pPaths)[(*pPathCount)++] = path;
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
        *pExpressions = realloc(*pExpressions, sizeof(Expression) * ((*pExpressionCount)+1));
        (*pExpressions)[*pExpressionCount].type = type;
        if (type == NAME || type == TYPE || type == USER) {
            if (i+1 >= argc) {
                printf("find: missing argument to `%s'\n", argv[i]);
                exit(1);
            }
            (*pExpressions)[*pExpressionCount].argument = argv[i+1];
            i++;
        } else {
            (*pExpressions)[*pExpressionCount].argument = "";
        }
        (*pExpressionCount)++;
    } 
    if (*pPathCount == 0) {
        *pPathCount = 1;
        *pPaths = malloc(sizeof(char*));
        (*pPaths)[0] = malloc(sizeof(char) * 2);
        (*pPaths)[0][0] = '.';
        (*pPaths)[0][1] = '\0';
    } 
    int printOrLsUsed = 0;
    for (int i = 0; i < (*pExpressionCount); i++) {
        if ((*pExpressions)[i].type == PRINT || (*pExpressions)[i].type == LS) {
            printOrLsUsed = 1;
            break;
        }
    }
    if (!printOrLsUsed) {
        *pExpressionCount = (*pExpressionCount) + 1;
        *pExpressions = realloc(*pExpressions, sizeof(Expression) * (*pExpressionCount));
        (*pExpressions)[(*pExpressionCount)-1].type = PRINT;
        (*pExpressions)[(*pExpressionCount)-1].argument = "";
    }
}

void iterateThroughDirectoryTree(char *path, Expression *expressions, int expressionCount, struct stat fileStat) {
    if (S_ISDIR(fileStat.st_mode)) {
        DIR *dir = opendir(path);
        if (dir == NULL) {
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
            if (stat(newPath, &newFileStat) == 0) // if the file can be accessed
                iterateThroughDirectoryTree(newPath, expressions, expressionCount, newFileStat);
            free(newPath);
        }
        closedir(dir);
    } 
    applyTestsAndActions(expressions, expressionCount, path, fileStat);
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
                    if (S_ISREG(fileStat.st_mode)) {
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "d") == 0) {
                    if (S_ISDIR(fileStat.st_mode)) {
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "l") == 0) {
                    if (S_ISLNK(fileStat.st_mode)) {
                        break;
                    } else {
                        return;
                    }
                } else if(strcmp(expressions[i].argument, "c") == 0) {
                    if (S_ISCHR(fileStat.st_mode)) {
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "b") == 0) {
                    if (S_ISBLK(fileStat.st_mode)) {
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "p") == 0) {
                    if (S_ISFIFO(fileStat.st_mode)) {
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "s") == 0) {
                    if (S_ISSOCK(fileStat.st_mode)) {
                        break;
                    } else {
                        return;
                    }
                } else {
                    printf("find: Unknown argument to -type: %s\n", expressions[i].argument);
                    exit(1);
                }
            case USER: {
                    char *userNameOrId = expressions[i].argument;
                    struct passwd *user = getpwnam(userNameOrId);
                    if (user == NULL && isdigit(userNameOrId[0])) {
                        user = getpwuid(atoi(userNameOrId));
                    }
                    if (user == NULL) {
                        printf("find: '%s' is not the name of a known user\n", userNameOrId);
                        exit(1);
                    }
                    if (user->pw_uid == fileStat.st_uid) {
                        break;
                    } else {
                        return;
                    }
                }
                break;
            default:
                break;
        }
    }
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