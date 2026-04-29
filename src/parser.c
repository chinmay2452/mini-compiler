#include <stdio.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/error.h"

int parse(Token tokens[], int count) {
    FILE *fout = fopen("output.txt", "a");

    int i = 0;

    while(i < count) {

        // Declaration
        if(strcmp(tokens[i].type, "KEYWORD") == 0) {

            if(i+3 >= count) {
                syntaxError(fout, "Incomplete declaration");
                fclose(fout);
                return 0;
            }

            if(strcmp(tokens[i+1].type, "IDENTIFIER") != 0) {
                syntaxError(fout, "Expected identifier");
                fclose(fout);
                return 0;
            }

            if(strcmp(tokens[i+2].value, "=") != 0) {
                syntaxError(fout, "Expected '='");
                fclose(fout);
                return 0;
            }

            if(strcmp(tokens[i+3].type, "NUMBER") != 0 &&
               strcmp(tokens[i+3].type, "IDENTIFIER") != 0) {
                syntaxError(fout, "Invalid value");
                fclose(fout);
                return 0;
            }

            i += 4;
        }

        // Assignment
        else if(strcmp(tokens[i].type, "IDENTIFIER") == 0) {

            if(i+2 >= count || strcmp(tokens[i+1].value, "=") != 0) {
                syntaxError(fout, "Invalid assignment");
                fclose(fout);
                return 0;
            }

            i += 2;

            if(strcmp(tokens[i].type, "IDENTIFIER") != 0 &&
               strcmp(tokens[i].type, "NUMBER") != 0) {
                syntaxError(fout, "Invalid expression");
                fclose(fout);
                return 0;
            }

            i++;

            if(i < count && strcmp(tokens[i].value, "+") == 0) {
                i++;
                if(strcmp(tokens[i].type, "IDENTIFIER") != 0 &&
                   strcmp(tokens[i].type, "NUMBER") != 0) {
                    syntaxError(fout, "Invalid operand");
                    fclose(fout);
                    return 0;
                }
                i++;
            }
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