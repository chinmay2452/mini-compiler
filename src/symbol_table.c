#include <stdio.h>
#include <string.h>
#include "../include/symbol_table.h"
#include "../include/error.h"

#define MAX 100

struct Symbol {
    char name[20];
    char type[10];
    char value[20];
} table[MAX];

int count = 0;

int exists(char *name) {
    for(int i = 0; i < count; i++) {
        if(strcmp(table[i].name, name) == 0)
            return 1;
    }
    return 0;
}

void addSymbol(char *type, char *name, char *value) {

    FILE *fout = fopen("output.txt", "a");

    if(exists(name)) {
        semanticError(fout, "Duplicate variable");
        fclose(fout);
        return;
    }

    strcpy(table[count].type, type);
    strcpy(table[count].name, name);
    strcpy(table[count].value, value);
    count++;

    fclose(fout);
}

void printTableToFile(FILE *fout) {
    fprintf(fout, "Name\tType\tValue\n");

    for(int i = 0; i < count; i++) {
        fprintf(fout, "%s\t%s\t%s\n",
            table[i].name,
            table[i].type,
            table[i].value);
    }
}