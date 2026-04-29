#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"
#include "../include/error.h"

int isKeyword(char *str) {
    return strcmp(str, "int") == 0 || strcmp(str, "float") == 0;
}

int isNumber(char *str) {
    int dot = 0;
    for(int i = 0; str[i]; i++) {
        if(str[i] == '.') dot++;
        else if(!isdigit(str[i])) return 0;
    }
    return dot <= 1;
}

int tokenize(char *code, Token tokens[]) {
    int count = 0;
    FILE *fout = fopen("output.txt", "a");
    int i = 0;

    while(code[i] != '\0') {
        if(isspace(code[i])) {
            i++;
            continue;
        }

        if(code[i] == ';') {
            strcpy(tokens[count].type, "SEMICOLON");
            strcpy(tokens[count].value, ";");
            count++;
            i++;
            continue;
        }

        if(code[i] == '=' || code[i] == '+' || code[i] == '-' || code[i] == '*' || code[i] == '/') {
            strcpy(tokens[count].type, "OPERATOR");
            char op[2] = {code[i], '\0'};
            strcpy(tokens[count].value, op);
            count++;
            i++;
            continue;
        }

        if(isalpha(code[i])) {
            int j = 0;
            char buffer[50];
            while(isalpha(code[i]) || isdigit(code[i])) {
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';

            if(isKeyword(buffer))
                strcpy(tokens[count].type, "KEYWORD");
            else
                strcpy(tokens[count].type, "IDENTIFIER");
            
            strcpy(tokens[count].value, buffer);
            count++;
            continue;
        }

        if(isdigit(code[i]) || code[i] == '.') {
            int j = 0;
            char buffer[50];
            while(isdigit(code[i]) || code[i] == '.') {
                buffer[j++] = code[i++];
            }
            buffer[j] = '\0';

            if(isNumber(buffer)) {
                strcpy(tokens[count].type, "NUMBER");
                strcpy(tokens[count].value, buffer);
                count++;
            } else {
                lexicalError(fout, buffer);
            }
            continue;
        }

        // Unknown character
        char unknown[2] = {code[i], '\0'};
        lexicalError(fout, unknown);
        i++;
    }

    fclose(fout);
    return count;
}