%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

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
["]                 TOKEN(QUOTE)
[-_/\.a-zA-Z0-9]+   {
                        // printf("%s\n", yytext);
                        yylval.str = strdup(yytext);
                        return ID;
                    }
\"[-_/\.a-zA-Z0-9 ]+\"         {
                        int len = strlen(yytext);
                        char *dup = (char *)malloc(len);
                        strncpy(dup, yytext + 1, len - 2);
                        *(dup + len - 2) = '\0';
                        yylval.str = dup;
                        return ID;
                    }
"&"                 TOKEN(AMPERSAND)
.                   TOKEN(BAD_TOKEN)


%%






