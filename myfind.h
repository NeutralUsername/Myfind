#ifndef MYFIND_H
#define MYFIND_H
#include <sys/stat.h>

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
typedef struct ProcessedArguments {
    int expressionCount;
    Expression *expressions;
    int pathCount;
    char **paths;
} ProcessedArguments;
ExpressionType getExpressionType(char *expression);
ProcessedArguments commandLineParsingAndValidation(int argc, char *argv[]);
void iterateThroughDirectoryTree(char *path, Expression *expressions, int expressionCount, struct stat fileStat);
void applyTestsAndActions(Expression *expressions, int expressionCount, char *path, struct stat fileStat);
int isValidType(char *type);
int isValidUser(char *user);
#endif