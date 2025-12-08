#ifndef LOCATION_H
#define LOCATION_H

#include <stdio.h>


struct DerivationStep {
    char *derivation;
    struct DerivationStep* next;
    struct DerivationStep* prev;
};


extern struct DerivationStep* lastDerivationStep;



void markStepDerivation(char *derivation);


void writeDerivationToFile(char *filePath);




extern int locationX;  
extern int locationY;   
extern int tokenLength; 


int getLocationX();    
int getLocationY();     


void updateLocationX(char *lexeme);
void updateLocationY(char *lexeme);

int countNewLines(char *text);
int lengthOfLastLine(char *text);
int lastIndexofNewLine(char *text);

#endif 
