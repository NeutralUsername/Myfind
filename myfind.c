#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
    for (int i = 0; i < expressionCount; i++) {
        printExpression(&expressions[i], i);
    }
    for (int i = 0; i < pathCount; i++) {
        printf("Path: %s\n", paths[i]);
    }
    return 0;
}

void processArgs(int argc, char *argv[], Expression **expressions, int *expressionCount, char ***paths, int *pathCount) {
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-') {
            if (*expressionCount == 0) {
                *paths = realloc(*paths, sizeof(char*) * (*pathCount)+1);
                (*paths)[(*pathCount)++] = argv[i];
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
        *expressions = realloc(*expressions, sizeof(Expression) * (*expressionCount)+1);
        (*expressions)[*expressionCount].type = type;
        if (type == NAME || type == TYPE || type == USER) {
            (*expressions)[*expressionCount].argument = argv[++i];
        }
        (*expressionCount)++;
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
