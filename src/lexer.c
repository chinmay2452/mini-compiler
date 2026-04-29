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
    char *token = strtok(code, " ;=+\n");
    int count = 0;

    FILE *fout = fopen("output.txt", "a");

    while(token != NULL) {

        if(isKeyword(token))
            strcpy(tokens[count].type, "KEYWORD");

        else if(isNumber(token))
            strcpy(tokens[count].type, "NUMBER");

        else if(isalpha(token[0]))
            strcpy(tokens[count].type, "IDENTIFIER");

        else {
            lexicalError(fout, token);
            token = strtok(NULL, " ;=+\n");
            continue;
        }

        strcpy(tokens[count].value, token);
        count++;

        token = strtok(NULL, " ;=+\n");
    }

    fclose(fout);
    return count;
}