#include <stdio.h>
#include <string.h>

typedef enum expressionType {
    PRINT,
    LS,
    NAME,
    TYPE,
    USER,
} expressionType;


typedef struct Expression {
    expressionType type;
    char *argument;
} Expression;

int main(int argc, char *argv[])
{
    int expressionCount = 0;
    Expression *expressions = NULL;
    int pathCount = 0;
    char *path = "/";
    char **paths = malloc(sizeof(char*));
    paths[0] = path;
    for (int i = 0; i < argc; i++)
    {
        if ( i == 0) {
            continue;
        } 

        if (expressionCount == 0) {
            if (argv[i][0] != '-') {
                paths = realloc(paths, sizeof(char*) * pathCount+1);
                paths[pathCount++] = argv[i];
            } 
        } else {
            if (argv[i][0] != '-') {
                printf("find: path must preceed expression: `%s'\n", argv[i]);
                return 1;
            }
        }

        if (validExpression(argv[i]) == 0) {
            printf("find: unknown predicate `%s'\n", argv[i]);
            return 1;
        }
        expressions = realloc(expressions, sizeof(Expression) * expressionCount+1);
        expressions[expressionCount].type = getExpressionType(argv[i]);
        if (expressions[expressionCount].type == NAME || expressions[expressionCount].type == TYPE || expressions[expressionCount].type == USER) {
            expressions[expressionCount].argument = argv[++i];
        }
        expressionCount++;
    } 
    return 0;
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
}

int validExpression (char *expression) {
    if (strcmp(expression, "-print") != 0)
        if (strcmp(expression, "-ls") != 0)
            if (strcmp(expression, "-name") != 0)
                if (strcmp(expression, "-type") != 0)
                    if (strcmp(expression, "-user") != 0){
                        return 0;
                    }
    return 1;
}