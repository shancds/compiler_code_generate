#include "error.h"

void initializeErrorAnalyzer()
{
    errorMessageFile = fopen("./output/erros.txt", "w");
    if (errorMessageFile == NULL)
    {
        printf("Error opening file for writing!\n");
    } 
}

void finalizeErrorAnalyzer()
{ 
    if (errorMessageFile != NULL)
    {
        fclose(errorMessageFile);
    }
}

void captureErrorAnalyzerMessage(char *message)
{
    fprintf(errorMessageFile, "%s\n", message);
}

int hasErrors()
{
    fseek(errorMessageFile, 0, SEEK_END); // Move to the end of the file
    long fileSizeBefore = ftell(errorMessageFile);

    return fileSizeBefore > 0;
}