#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"
#include "../include/error.h"

static int isIdentStart(unsigned char c) {
    return isalpha(c) || c == '_';
}

static int isIdentPart(unsigned char c) {
    return isalnum(c) || c == '_';
}

static int isKeywordInt(const char *str) {
    return strcmp(str, "int") == 0;
}

static void pushToken(Token tokens[], int *count, const char *type, const char *value) {
    if (*count >= MAX_TOKENS) return;
    strncpy(tokens[*count].type, type, sizeof(tokens[*count].type) - 1);
    tokens[*count].type[sizeof(tokens[*count].type) - 1] = '\0';
    strncpy(tokens[*count].value, value, sizeof(tokens[*count].value) - 1);
    tokens[*count].value[sizeof(tokens[*count].value) - 1] = '\0';
    (*count)++;
}

int tokenize(char *code, Token tokens[]) {
    int count = 0;
    int i = 0;

    while (code[i] != '\0') {
        unsigned char c = (unsigned char)code[i];

        if (isspace(c)) {
            i++;
            continue;
        }

        if (c == ';') {
            pushToken(tokens, &count, "SEMICOLON", ";");
            i++;
            continue;
        }

        if (c == '(') {
            pushToken(tokens, &count, "LPAREN", "(");
            i++;
            continue;
        }

        if (c == ')') {
            pushToken(tokens, &count, "RPAREN", ")");
            i++;
            continue;
        }

        if (c == '=' || c == '+' || c == '-' || c == '*' || c == '/') {
            char op[2] = {(char)c, '\0'};
            pushToken(tokens, &count, "OPERATOR", op);
            i++;
            continue;
        }

        if (isIdentStart(c)) {
            int j = 0;
            char buffer[50];
            while (code[i] != '\0' && isIdentPart((unsigned char)code[i])) {
                if (j < (int)sizeof(buffer) - 1) buffer[j++] = code[i];
                i++;
            }
            buffer[j] = '\0';

            if (isKeywordInt(buffer)) pushToken(tokens, &count, "KEYWORD", buffer);
            else pushToken(tokens, &count, "IDENTIFIER", buffer);
            continue;
        }

        if (isdigit(c)) {
            int j = 0;
            char buffer[50];
            while (code[i] != '\0' && isdigit((unsigned char)code[i])) {
                if (j < (int)sizeof(buffer) - 1) buffer[j++] = code[i];
                i++;
            }
            buffer[j] = '\0';

            pushToken(tokens, &count, "NUMBER", buffer);
            continue;
        }

        // Unknown character: report and skip (keeps token stream aligned for valid inputs).
        {
            char unknown[2] = {(char)c, '\0'};
            lexicalError(stderr, unknown);
        }
        i++;
    }

    return count;
}