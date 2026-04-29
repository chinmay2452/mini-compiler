#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>

// Insert a symbol if missing (no duplicates).
// `type` should be something like "variable".
// `value` can be "-" if unknown.
void addSymbol(const char *type, const char *name, const char *value);

// Update value for an existing symbol. If the symbol is missing, it is inserted.
void setSymbolValue(const char *name, const char *value);

// Number of symbols currently stored.
int symbolCount(void);

void printTableToFile(FILE *fout);
void printTable();

#endif