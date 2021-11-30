%{
#include <stdio.h>
#include "job.h"

extern int yylex();
extern int yyparse();
extern void yyerror(char *);

job_t current_job = NULL;
proc_t current_proc = NULL;
    
%}

%union {
    int c;
    char * str;
}

%token<c>   AMPERSAND GREATER LESS PIPE NEWLINE BAD_TOKEN QUOTE SIMECOLON
%token<str> ID
%start pipeline_list


%%

cmd: ID {
            if (current_job == NULL) {
                current_job = create_job();
            }

            if (current_proc == NULL) {
                current_proc = create_process();
            }

            process_add_arg(current_proc, yylval.str);
        
        }
;
arg: ID {   
            printf("%s\n", yylval.str);
            printf("%d\n", yylval.str == $1);
            process_add_arg(current_proc, $1);
        }
;
arg_list: arg           {}
    | arg_list arg      {}
;
simple_command: cmd arg_list {}
;
simple_command_list: simple_command             {}
    | simple_command_list PIPE simple_command   {}
;
redirection: LESS ID    {}
    | GREATER ID          {}
    | '2' GREATER ID      {}
;
redirection_list: /* empty */
    | redirection_list redirection  {}
;
background: /* empty */
    | AMPERSAND {}
;
terminator: NEWLINE     {}
    | SIMECOLON         {}
;
pipeline: terminator    {}
    | simple_command_list redirection_list background terminator    {}
;
pipeline_list: pipeline         {}
    | pipeline_list pipeline    {}
;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}