#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

void lexicalError(FILE *fout, char *msg);

void syntaxError(FILE *fout, char *msg);

void semanticError(FILE *fout, char *msg);
#endif