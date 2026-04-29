#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>

void addSymbol(char *type, char *name, char *value);
void printTableToFile(FILE *fout);
void printTable();

#endif