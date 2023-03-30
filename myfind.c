#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <fnmatch.h>
#include <ctype.h>


typedef enum ExpressionType {
    PRINT,
    LS,
    NAME,
    TYPE,
    USER,
    invalid
} ExpressionType;

typedef struct Expression {
    ExpressionType type;
    char *argument;
} Expression;

ExpressionType getExpressionType(char *expression);
void processArgs(int argc, char *argv[], Expression **expressions, int *expressionCount, char ***paths, int *pathCount);
void iterateThroughDirectoryTree(char *path, Expression *expressions, int expressionCount, struct stat fileStat);

int main(int argc, char *argv[]) {
    int expressionCount = 0;
    Expression *expressions = NULL;
    int pathCount = 0;
    char **paths = NULL;
    processArgs(argc, argv, &expressions, &expressionCount, &paths, &pathCount);
    if (pathCount == 0) {
        pathCount = 1;
        paths = malloc(sizeof(char*));
        char *currentWorkingDirectory = malloc(1024);
        getcwd(currentWorkingDirectory, 1024);
        paths[0] = currentWorkingDirectory;
    } 
    for (int i = 0; i < pathCount; i++) {
        struct stat fileStat;
        if (stat(paths[i], &fileStat) < 0) { //if the (initial) path does not exist
            printf("find: '%s': No such file or directory\n", paths[i]); 
            continue;
        }
        iterateThroughDirectoryTree(paths[i], expressions, expressionCount, fileStat);
    }
    return 0;
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
    for (int i = 0; i < expressionCount; i++) {
        switch (expressions[i].type) {
            case PRINT:
                printf("%s\n", path);
                break;
            case LS:
                {
                    printf("%ld\t", fileStat.st_ino);
                    printf("%ld\t", fileStat.st_blocks);
                    printf("%o\t", fileStat.st_mode);
                    printf("%ld\t", fileStat.st_nlink);
                    printf("%d\t", fileStat.st_uid);
                    printf("%d\t", fileStat.st_gid);
                    printf("%ld\t", fileStat.st_size);
                    printf("%s\t", ctime(&fileStat.st_mtime));
                    printf("%s\n", path);
                }
                break;
            case NAME:
                if(fnmatch(expressions[i].argument, path, 0) == 0) {
                    printf("%s\n", path);
                    break;
                } else {
                    return;
                }
            case TYPE:
                if (strcmp(expressions[i].argument, "f") == 0) {
                    if (S_ISREG(fileStat.st_mode)) {
                        printf("%s\n", path);
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "d") == 0) {
                    if (S_ISDIR(fileStat.st_mode)) {
                        printf("%s\n", path);
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "l") == 0) {
                    if (S_ISLNK(fileStat.st_mode)) {
                        printf("%s\n", path);
                        break;
                    } else {
                        return;
                    }
                } else if(strcmp(expressions[i].argument, "c") == 0) {
                    if (S_ISCHR(fileStat.st_mode)) {
                        printf("%s\n", path);
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "b") == 0) {
                    if (S_ISBLK(fileStat.st_mode)) {
                        printf("%s\n", path);
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "p") == 0) {
                    if (S_ISFIFO(fileStat.st_mode)) {
                        printf("%s\n", path);
                        break;
                    } else {
                        return;
                    }
                } else if (strcmp(expressions[i].argument, "s") == 0) {
                    if (S_ISSOCK(fileStat.st_mode)) {
                        printf("%s\n", path);
                        break;
                    } else {
                        return;
                    }
                } else {
                    printf("find: Unknown argument to -type: %s\n", expressions[i].argument);
                    exit(1);
                }
            case USER:
                {
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
                        printf("%s\n", path);
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

void processArgs(int argc, char *argv[], Expression **pExpressions, int *pExpressionCount, char ***pPaths, int *pPathCount) {
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-') {
            if (*pExpressionCount == 0) {
                *pPaths = realloc(*pPaths, sizeof(char*) * (*pPathCount)+1);
                (*pPaths)[(*pPathCount)++] = argv[i];
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
        *pExpressions = realloc(*pExpressions, sizeof(Expression) * (*pExpressionCount)+1);
        (*pExpressions)[*pExpressionCount].type = type;
        if (type == NAME || type == TYPE || type == USER) {
            (*pExpressions)[*pExpressionCount].argument = argv[++i];
        }
        (*pExpressionCount)++;
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