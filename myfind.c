#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
void printExpression(Expression *expression, int i);
void processArgs(int argc, char *argv[], Expression **expressions, int *expressionCount, char ***paths, int *pathCount);
void printPathsAndExpressions(char **paths, int pathCount, Expression *expressions, int expressionCount);
void iterateThroughDirectoryTree(char *path, Expression *expressions, int expressionCount);
int isDir(const char* fileName);

int main(int argc, char *argv[])
{
    int expressionCount = 0;
    Expression *expressions = NULL;
    int pathCount = 0;
    char **paths = NULL;
    processArgs(argc, argv, &expressions, &expressionCount, &paths, &pathCount);
    if (pathCount == 0) {
        pathCount = 1;
        paths = malloc(sizeof(char*));
        paths[0] = "/";
    } 
    printPathsAndExpressions(paths, pathCount, expressions, expressionCount);
    for (int i = 0; i < pathCount; i++) {
        iterateThroughDirectoryTree(paths[i], expressions, expressionCount);
    }
    return 0;
}

void iterateThroughDirectoryTree(char *path, Expression *expressions, int expressionCount) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("find: cannot access `%s': No such file or directory\n", path);
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char *newPath = malloc(strlen(path) + strlen(entry->d_name) + 2);
        strcpy(newPath, path);
        if (path[strlen(path)-1] != '/')
            strcat(newPath, "/");
        strcat(newPath, entry->d_name);
        printf("%s\n", newPath);
        if (isDir(newPath))
            iterateThroughDirectoryTree(newPath, expressions, expressionCount);
        free(newPath);
    }
    closedir(dir);
}

int isDir(const char* name) {
    struct stat path;
    stat(name, &path);
    return S_ISDIR(path.st_mode);
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

void printExpression(Expression *expression, int i) {
    printf("Expression %d: ", i);
    switch (expression->type) {
        case PRINT:
            printf("PRINT\n");
            break;
        case LS:
            printf("LS\n");
            break;
        case NAME:
            printf("NAME: %s\n", expression->argument);
            break;
        case TYPE:  
            printf("TYPE: %s\n", expression->argument);
            break;
        case USER:  
            printf("USER: %s\n", expression->argument);
            break;
        default:
            printf("Invalid expression\n");
    }
}

void printPathsAndExpressions(char **paths, int pathCount, Expression *expressions, int expressionCount) {
     for (int i = 0; i < expressionCount; i++) {
        printExpression(&expressions[i], i);
    }
    for (int i = 0; i < pathCount; i++) {
        printf("Path: %s\n", paths[i]);
    }
}