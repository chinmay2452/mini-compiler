#include <stdio.h>
#include "../include/error.h"

void lexicalError(FILE *fout, char *msg) {
    fprintf(fout, "Lexical Error: %s\n", msg);
}

void syntaxError(FILE *fout, char *msg) {
    fprintf(fout, "Syntax Error: %s\n", msg);
}

void semanticError(FILE *fout, char *msg) {
    fprintf(fout, "Semantic Error: %s\n", msg);
}