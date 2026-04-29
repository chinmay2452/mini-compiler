#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/symbol_table.h"
#include "include/utils.h"

int main() {

    FILE *fin = fopen("data/input.txt", "r");
    FILE *fout = fopen("data/output.txt", "w");

    if (!fin || !fout) {
        printf("File error\n");
        return 1;
    }

    // Read the entire file into memory (simple + reliable for small projects).
    if (fseek(fin, 0, SEEK_END) != 0) {
        printf("File error\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    long size = ftell(fin);
    if (size < 0) {
        printf("File error\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    rewind(fin);

    char *code = (char *)malloc((size_t)size + 1);
    if (!code) {
        printf("Memory error\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    size_t readBytes = fread(code, 1, (size_t)size, fin);
    code[readBytes] = '\0';

    fclose(fin);

    Token tokens[MAX_TOKENS];
    int count = tokenize(code, tokens);
    free(code);

    fprintf(fout, "TOKENS:\n");
    for (int i = 0; i < count; i++) {
        fprintf(fout, "%s : %s\n", tokens[i].type, tokens[i].value);
    }

    if (parse(tokens, count)) {
        fprintf(fout, "\nSyntax Correct\n");

        for (int i = 0; i < count; i++) {
            if (strcmp(tokens[i].type, "KEYWORD") == 0) {
                addSymbol(tokens[i].value,
                          tokens[i+1].value,
                          tokens[i+3].value);
            }
        }

        fprintf(fout, "\nSymbol Table:\n");
        printTableToFile(fout);
    } else {
        fprintf(fout, "\nSyntax Error\n");
    }

    fclose(fout);
    return 0;
}