%{
#include <stdio.h>
#include "job.h"
#include "err.h"

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

%token<c>   AMPERSAND GREATER LESS PIPE NEWLINE BAD_TOKEN SIMECOLON
%token<str> ID
%start pipeline_list


%%

cmd: ID {
            if (current_job == NULL) {
                current_job = create_job();
            }

            if (current_proc == NULL) {
                // debug("gen new proc...");
                current_proc = create_process();
            }

            process_add_arg(current_proc, $1);
        
        }
;
arg: ID {   
            // debug("arg: %s", $1);
            process_add_arg(current_proc, $1);
        }
;
arg_list: /* empty */   {}
    | arg_list arg      {}
;
simple_command: cmd arg_list {
                                // debug("add process...");
                                job_add_process(current_job, current_proc);
                                current_proc = NULL;
                            }
;
simple_command_list: simple_command {}
    | simple_command_list PIPE simple_command   {}
;
redirection: LESS ID    {current_job->in_file = $2;}
    | GREATER ID          {current_job->out_file = $2;}
    | '2' GREATER ID      {current_job->err_file = $3;}
;
redirection_list: /* empty */
    | redirection_list redirection  {}
;
background: /* empty */
    | AMPERSAND {
        current_job->fg = 0;
    }
;
terminator: NEWLINE     {}
    | SIMECOLON         {}
;
pipeline: terminator    {}
    | simple_command_list redirection_list background terminator    {
        debug("start execution");
        int res = execute_job(current_job);
        debug("the result of execution is: %d", res);
        current_job = NULL; // only for test
    }
;
pipeline_list: pipeline         {}
    | pipeline_list pipeline    {}
;


%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}