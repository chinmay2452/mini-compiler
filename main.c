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

    if (parse(tokens, count)) {
        fprintf(fout, "\nSyntax Correct\n");

        // Build symbol table:
        // - Add every IDENTIFIER once (type = "variable", value = "-")
        // - Bonus: store value for simple assignments: a = 2; or int a = 2;
        for (int i = 0; i < count; i++) {
            if (strcmp(tokens[i].type, "IDENTIFIER") == 0) {
                addSymbol("variable", tokens[i].value, "-");
            }
        }

        for (int i = 0; i + 3 < count; i++) {
            // Pattern: IDENTIFIER = NUMBER ;
            if (strcmp(tokens[i].type, "IDENTIFIER") == 0 &&
                strcmp(tokens[i + 1].type, "OPERATOR") == 0 &&
                strcmp(tokens[i + 1].value, "=") == 0 &&
                strcmp(tokens[i + 2].type, "NUMBER") == 0 &&
                strcmp(tokens[i + 3].type, "SEMICOLON") == 0) {
                setSymbolValue(tokens[i].value, tokens[i + 2].value);
            }

            // Pattern: int IDENTIFIER = NUMBER ;
            // Only set a value when the RHS is a single literal number (no expression).
            if (i + 4 < count &&
                strcmp(tokens[i].type, "KEYWORD") == 0 &&
                strcmp(tokens[i].value, "int") == 0 &&
                strcmp(tokens[i + 1].type, "IDENTIFIER") == 0 &&
                strcmp(tokens[i + 2].type, "OPERATOR") == 0 &&
                strcmp(tokens[i + 2].value, "=") == 0 &&
                strcmp(tokens[i + 3].type, "NUMBER") == 0 &&
                strcmp(tokens[i + 4].type, "SEMICOLON") == 0) {
                // Ensure identifier exists and set its value.
                addSymbol("variable", tokens[i + 1].value, "-");
                setSymbolValue(tokens[i + 1].value, tokens[i + 3].value);
            }
        }

        if (symbolCount() > 0) {
            fprintf(fout, "\nSymbol Table:\n");
            printTableToFile(fout);
        }
    } else {
        fprintf(fout, "\nSyntax Error\n");
    }

    fclose(fout);
    return 0;
}