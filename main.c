#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/symbol_table.h"
#include "include/utils.h"

int main() {

    const char *inputPath = "data/input.txt";
    FILE *fin = fopen(inputPath, "r");
    if (!fin) {
        // Fallback for when the user edits root-level input.txt
        inputPath = "input.txt";
        fin = fopen(inputPath, "r");
    }
    FILE *fout = fopen("data/output.txt", "w");

    if (!fin) {
        printf("Error: could not open data/input.txt or input.txt\n");
        if (fout) fclose(fout);
        return 1;
    }

    if (!fout) {
        printf("Error: could not open data/output.txt\n");
        fclose(fin);
        return 1;
    }

    // Read the latest input fresh on every run, line by line.
    printf("DEBUG: Reading fresh input...\n");
    printf("DEBUG: Using input path: %s\n", inputPath);

    size_t capacity = 256;
    size_t length = 0;
    char *code = (char *)malloc(capacity);
    if (!code) {
        printf("Memory error\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }
    code[0] = '\0';

    char line[256];
    while (fgets(line, sizeof(line), fin)) {
        size_t lineLen = strlen(line);
        printf("Line: %s", line);

        if (length + lineLen + 1 > capacity) {
            while (length + lineLen + 1 > capacity) {
                capacity *= 2;
            }
            char *newCode = (char *)realloc(code, capacity);
            if (!newCode) {
                printf("Memory error\n");
                free(code);
                fclose(fin);
                fclose(fout);
                return 1;
            }
            code = newCode;
        }

        memcpy(code + length, line, lineLen);
        length += lineLen;
        code[length] = '\0';
    }

    fclose(fin);

    Token tokens[MAX_TOKENS];
    int count = tokenize(code, tokens);
    free(code);

    fprintf(fout, "TOKENS:\n");
    for (int i = 0; i < count; i++) {
        fprintf(fout, "%s : %s\n", tokens[i].type, tokens[i].value);
    }

    fclose(fout); // close before parsing to avoid file pointer conflicts

    if (parse(tokens, count)) {
        fout = fopen("data/output.txt", "a");
        if (fout) {
            fprintf(fout, "\nSyntax Correct\n\n");
            printTableToFile(fout);
            fclose(fout);
        }
    }
    return 0;
}