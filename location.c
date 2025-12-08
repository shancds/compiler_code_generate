#include <stdlib.h>
#include <string.h>
#include "location.h"

// ================== DERIVATION PROCESSOR ==================

// Global pointer to the last derivation step
struct DerivationStep* lastDerivationStep = NULL;

void markStepDerivation(char *derivation){
   
    struct DerivationStep* newDerivationStep = (struct DerivationStep*)malloc(sizeof(struct DerivationStep));
    
  
    newDerivationStep->derivation = derivation;
    newDerivationStep->next = NULL; 
    newDerivationStep->prev = NULL; 

   
    newDerivationStep->prev = lastDerivationStep;
    lastDerivationStep = newDerivationStep; 
}

void writeDerivationToFile(char *filePath){ 
    struct DerivationStep* current = lastDerivationStep;
    
    
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        perror("Error opening the derivation out file.");
        return;
    }

    
    while(current != NULL){ 
        fprintf(file, "%s\n", current->derivation);
        current = current->prev;
    }

    fclose(file);
}


// ================== LOCATION TRACKING ==================




int getLocationX(){
    return locationX - tokenLength;
}

int getLocationY(){
    return locationY;
}

void updateLocationX(char *lexeme){
    locationX += lengthOfLastLine(lexeme);
    tokenLength = strlen(lexeme);
}

void updateLocationY(char *lexeme){
    locationY += countNewLines(lexeme);
}

int countNewLines(char *text) {
    int count = 0;
    char *ptr = text;
    
    while (*ptr != '\0') {
        if (*ptr == '\n') {
            count++;
        }
        ptr++;
    }
    
    return count;
}

int lengthOfLastLine(char *text) {
    return (int)strlen(text) - lastIndexofNewLine(text) - 2;
}

int lastIndexofNewLine(char *text) {
    int lastIndex = -1;
    int i = 0;
    
    while (text[i] != '\0') {
        if (text[i] == '\n') {
            lastIndex = i;
        }
        i++;
    }
    
    return lastIndex;
}
