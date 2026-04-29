#include <stdio.h>
#include <string.h>
#include "../include/symbol_table.h"

#define MAX_SYMBOLS 200

typedef struct Symbol {
    char name[50];
    char type[20];
    char value[20];
} Symbol;

static Symbol table[MAX_SYMBOLS];
static int count = 0;

static int findIndex(const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(table[i].name, name) == 0) return i;
    }
    return -1;
}

int symbolCount(void) {
    return count;
}

void addSymbol(const char *type, const char *name, const char *value) {
    if (!name || name[0] == '\0') return;
    if (findIndex(name) >= 0) return; // ignore duplicates
    if (count >= MAX_SYMBOLS) return;

    strncpy(table[count].name, name, sizeof(table[count].name) - 1);
    table[count].name[sizeof(table[count].name) - 1] = '\0';

    strncpy(table[count].type, type ? type : "variable", sizeof(table[count].type) - 1);
    table[count].type[sizeof(table[count].type) - 1] = '\0';

    strncpy(table[count].value, value ? value : "-", sizeof(table[count].value) - 1);
    table[count].value[sizeof(table[count].value) - 1] = '\0';

    count++;
}

void setSymbolValue(const char *name, const char *value) {
    if (!name || name[0] == '\0') return;
    int idx = findIndex(name);
    if (idx < 0) {
        addSymbol("variable", name, value ? value : "-");
        return;
    }

    strncpy(table[idx].value, value ? value : "-", sizeof(table[idx].value) - 1);
    table[idx].value[sizeof(table[idx].value) - 1] = '\0';
}

void printTableToFile(FILE *fout) {
    fprintf(fout, "Name\tType\tValue\n");
    for (int i = 0; i < count; i++) {
        fprintf(fout, "%s\t%s\t%s\n", table[i].name, table[i].type, table[i].value);
    }
}

void printTable() {
    printTableToFile(stdout);
}