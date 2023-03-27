#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef enum expressionType {
    PRINT,
    LS,
    NAME,
    TYPE,
    USER,
    invalid
} expressionType;


typedef struct Expression {
    expressionType type;
    char *argument;
} Expression;

expressionType getExpressionType(char *expression);
void printExpression(Expression *expression, int i);

int main(int argc, char *argv[])
{
    int expressionCount = 0;
    Expression *expressions = NULL;
    int pathCount = 1; //magic
    char **paths = malloc(sizeof(char*));
    paths[0] = "/";
    for (int i = 0; i < argc; i++)
    {
        if ( i == 0) {
            continue;
        } 

        if (argv[i][0] != '-') {
            if (expressionCount == 0) {
                pathCount = 0; //magic
                paths = realloc(paths, sizeof(char*) * pathCount+1);
                paths[pathCount++] = argv[i];
            } else {
                printf("find: path must preceed expression: `%s'\n", argv[i]);
                return 1;
            }
            continue;
        }

        expressionType type = getExpressionType(argv[i]);
        if (type == invalid) {
            printf("find: unknown predicate `%s'\n", argv[i]);
            return 1;
        }
        expressions = realloc(expressions, sizeof(Expression) * expressionCount+1);
        expressions[expressionCount].type = type;
        if (type == NAME || type == TYPE || type == USER) {
            expressions[expressionCount].argument = argv[++i];
        }
        expressionCount++;
    } 
    for (int i = 0; i < expressionCount; i++) {
        printExpression(&expressions[i], i);
    }
    for (int i = 0; i < pathCount; i++) {
        printf("Path: %s\n", paths[i]);
    }
    return 0;
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

expressionType getExpressionType(char *expression) {
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