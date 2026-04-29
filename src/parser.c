#include <stdio.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/error.h"

static int isValueToken(const Token *t) {
    return strcmp(t->type, "IDENTIFIER") == 0 || strcmp(t->type, "NUMBER") == 0;
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

// Parse an expression until SEMICOLON (does not consume the SEMICOLON).
// Grammar (informal): expr := ( '(' )* value ( (')' | op value | op '(' ... ) )*
// This is a simple validator (not precedence-aware), good for first-year projects.
static int parseExpressionUntilSemicolon(Token tokens[], int count, int *i, FILE *fout) {
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
            expectOperand = 0;
            (*i)++;
        } else {
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

int parse(Token tokens[], int count) {
    FILE *fout = fopen("data/output.txt", "a");
    if (!fout) return 0;

    int i = 0;

    while(i < count) {

        // Declaration
        if(strcmp(tokens[i].type, "KEYWORD") == 0) {

            // Expect: int IDENTIFIER = <expression> ;
            if (strcmp(tokens[i].value, "int") != 0) {
                syntaxError(fout, "Unknown keyword");
                fclose(fout);
                return 0;
            }

            if(i+4 >= count) {
                syntaxError(fout, "Incomplete declaration");
                fclose(fout);
                return 0;
            }

            if(strcmp(tokens[i+1].type, "IDENTIFIER") != 0) {
                syntaxError(fout, "Expected identifier");
                fclose(fout);
                return 0;
            }

            if(strcmp(tokens[i+2].type, "OPERATOR") != 0 || strcmp(tokens[i+2].value, "=") != 0) {
                syntaxError(fout, "Expected '='");
                fclose(fout);
                return 0;
            }

            i += 3; // now at start of expression
            if (!parseExpressionUntilSemicolon(tokens, count, &i, fout)) {
                fclose(fout);
                return 0;
            }

            if(i >= count || strcmp(tokens[i].type, "SEMICOLON") != 0) {
                syntaxError(fout, "Missing ';' after declaration");
                fclose(fout);
                return 0;
            }
            i++; // consume ';'
        }

        // Assignment
        else if(strcmp(tokens[i].type, "IDENTIFIER") == 0) {

            if(i+2 >= count || strcmp(tokens[i+1].value, "=") != 0) {
                syntaxError(fout, "Invalid assignment");
                fclose(fout);
                return 0;
            }

            i += 2;

            if (!parseExpressionUntilSemicolon(tokens, count, &i, fout)) {
                fclose(fout);
                return 0;
            }

            if(i >= count || strcmp(tokens[i].type, "SEMICOLON") != 0) {
                syntaxError(fout, "Missing ';' after assignment");
                fclose(fout);
                return 0;
            }

            i++;
        }

        else {
            syntaxError(fout, "Unknown statement");
            fclose(fout);
            return 0;
        }
    }

    fclose(fout);
    return 1;
}