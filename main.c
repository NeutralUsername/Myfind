#include "myfind.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    ProcessedArguments processedArgs = commandLineParsingAndValidation(argc, argv);
    for (int i = 0; i < processedArgs.pathCount; i++) {
        struct stat fileStat;
        if (lstat(processedArgs.paths[i], &fileStat) < 0) { //if the provided path does not exist (or cannot be accessed)
            printf("find: '%s': No such file or directory\n", processedArgs.paths[i]); 
            continue;
        }
        iterateThroughDirectoryTree(processedArgs.paths[i], processedArgs.expressions, processedArgs.expressionCount, fileStat);
    }

    free(processedArgs.expressions);
    for (int i = 0; i < processedArgs.pathCount; i++) {
        free(processedArgs.paths[i]);
    }
    free(processedArgs.paths);
    return 0;
}