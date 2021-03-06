%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "err.h"

#define TOKEN(token) {return token;}
#define EMPTY() { /* empty */ }
%}


%%

\n                  TOKEN(NEWLINE)
";"                 TOKEN(SIMECOLON)
[\t ]               EMPTY()
">"                 TOKEN(GREATER)
"<"                 TOKEN(LESS)
"|"                 TOKEN(PIPE)      
[-_/\.a-zA-Z0-9]+   {
                        // printf("%s\n", yytext);
                        yylval.str = strdup(yytext);
                        return ID;
                    }
\"(\\.|[^"\\])*\"         {
                        int len = strlen(yytext);
                        char *dup = (char *)malloc(len);
                        strncpy(dup, yytext + 1, len - 2);
                        *(dup + len - 2) = '\0';
                        yylval.str = dup;
                        return ID;
                    }
"&"                 TOKEN(AMPERSAND)
.                   {
                        error("bad token: %d", *yytext);
                    }


%%






