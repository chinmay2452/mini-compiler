#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/symbol_table.h"
#include "include/utils.h"

int main() {

    FILE *fin = fopen("input.txt", "r");
    FILE *fout = fopen("output.txt", "w");

    if (!fin || !fout) {
        printf("File error\n");
        return 1;
    }

    char code[500] = "";
    char line[100];

    while (fgets(line, sizeof(line), fin)) {
        strcat(code, line);
    }

    fclose(fin);

    Token tokens[MAX_TOKENS];
    int count = tokenize(code, tokens);

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