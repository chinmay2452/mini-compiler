#include <stdio.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/error.h"
#include "../include/symbol_table.h"

/*
    Helper checks
*/

static int isValueToken(const Token *t) {
    return strcmp(t->type, "IDENTIFIER") == 0 ||
           strcmp(t->type, "NUMBER") == 0;
}

static int isMathOperatorToken(const Token *t) {
    if (strcmp(t->type, "OPERATOR") != 0) return 0;

    return strcmp(t->value, "+") == 0 ||
           strcmp(t->value, "-") == 0 ||
           strcmp(t->value, "*") == 0 ||
           strcmp(t->value, "/") == 0;
}

static int isLParenToken(const Token *t) {
    return strcmp(t->type, "LPAREN") == 0;
}

static int isRParenToken(const Token *t) {
    return strcmp(t->type, "RPAREN") == 0;
}

/*
    Core Expression Parser
    (Used for both declaration and assignment)

    This simulates PDA behavior using:
    - parenDepth (stack-like tracking)
    - alternating operand/operator expectation
*/

static int parseExpression(Token tokens[], int *i, int count, FILE *fout) {

    int expectOperand = 1;
    int parenDepth = 0;

    while (*i < count && strcmp(tokens[*i].type, "SEMICOLON") != 0) {

        if (expectOperand) {

            if (isLParenToken(&tokens[*i])) {
                parenDepth++;
                (*i)++;
                continue;
            }

            if (!isValueToken(&tokens[*i])) {
                syntaxError(fout, "Invalid expression");
                return 0;
            }

            if (strcmp(tokens[*i].type, "IDENTIFIER") == 0) {
                if (!exists(tokens[*i].value)) {
                    semanticError(fout, "Undeclared variable usage");
                    return 0;
                }
            }

            expectOperand = 0;
            (*i)++;
        }

        else {

            if (isRParenToken(&tokens[*i])) {

                if (parenDepth <= 0) {
                    syntaxError(fout, "Unmatched ')'");
                    return 0;
                }

                parenDepth--;
                (*i)++;
                continue;
            }

            if (!isMathOperatorToken(&tokens[*i])) {
                syntaxError(fout, "Expected operator");
                return 0;
            }

            expectOperand = 1;
            (*i)++;
        }
    }

    if (expectOperand) {
        syntaxError(fout, "Expression ends with operator");
        return 0;
    }

    if (parenDepth != 0) {
        syntaxError(fout, "Unmatched '('");
        return 0;
    }

    return 1;
}

/*
    MAIN PARSER
*/

int parse(Token tokens[], int count) {

    FILE *fout = fopen("data/output.txt", "a");
    if (!fout) return 0;

    int i = 0;

    while (i < count) {

        /*
            DECLARATION:
            int a = expr;
        */
        if (strcmp(tokens[i].type, "KEYWORD") == 0) {

            if (i + 2 >= count) {
                syntaxError(fout, "Incomplete declaration");
                fclose(fout);
                return 0;
            }

            if (strcmp(tokens[i + 1].type, "IDENTIFIER") != 0) {
                syntaxError(fout, "Expected identifier");
                fclose(fout);
                return 0;
            }

            if (strcmp(tokens[i + 2].value, "=") != 0) {
                syntaxError(fout, "Expected '='");
                fclose(fout);
                return 0;
            }

            char *type = tokens[i].value;
            char *identifier = tokens[i + 1].value;

            if (exists(identifier)) {
                semanticError(fout, "Duplicate variable declaration");
                fclose(fout);
                return 0;
            }

            i += 3; // move to expression
            int exprStart = i;

            if (!parseExpression(tokens, &i, count, fout)) {
                fclose(fout);
                return 0;
            }

            if (i >= count || strcmp(tokens[i].type, "SEMICOLON") != 0) {
                syntaxError(fout, "Missing ';' after declaration");
                fclose(fout);
                return 0;
            }

            addSymbol(type, identifier, tokens[exprStart].value);

            i++;
        }

        /*
            ASSIGNMENT:
            a = expr;
        */
        else if (strcmp(tokens[i].type, "IDENTIFIER") == 0) {

            char *identifier = tokens[i].value;

            if (!exists(identifier)) {
                semanticError(fout, "Undeclared variable usage");
                fclose(fout);
                return 0;
            }

            if (i + 1 >= count || strcmp(tokens[i + 1].value, "=") != 0) {
                syntaxError(fout, "Invalid assignment");
                fclose(fout);
                return 0;
            }

            i += 2; // move to expression
            int exprStart = i;

            if (!parseExpression(tokens, &i, count, fout)) {
                fclose(fout);
                return 0;
            }

            if (i >= count || strcmp(tokens[i].type, "SEMICOLON") != 0) {
                syntaxError(fout, "Missing ';' after assignment");
                fclose(fout);
                return 0;
            }

            updateSymbol(identifier, tokens[exprStart].value);

            i++;
        }

        /*
            UNKNOWN
        */
        else {
            syntaxError(fout, "Unknown statement");
            fclose(fout);
            return 0;
        }
    }

    fclose(fout);
    return 1;
}