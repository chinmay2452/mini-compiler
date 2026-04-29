#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>

int exists(char *name);
void addSymbol(char *type, char *name, char *value);
void updateSymbol(char *name, char *value);
void printTableToFile(FILE *fout);

#endif