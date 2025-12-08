#ifndef ERROR_H
#define ERROR_H
#include <stdio.h>

extern FILE *errorMessageFile;
void captureErrorAnalyzerMessage(char *message);
void initializeErrorAnalyzer();
void finalizeErrorAnalyzer();
int hasErrors();

#endif