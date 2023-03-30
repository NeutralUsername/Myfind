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

ExpressionType getExpressionType(char *expression);
void commandLineParsingAndValidation(int argc, char *argv[], Expression **expressions, int *expressionCount, char ***paths, int *pathCount);
void iterateThroughDirectoryTree(char *path, Expression *expressions, int expressionCount, struct stat fileStat);
void applyTestsAndActions(Expression *expressions, int expressionCount, char *path, struct stat fileStat);
int isValidType(char *type);
int isValidUser(char *user);
#endif