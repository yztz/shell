%{
#include "job.h"
#include "err.h"

extern int yylex();
extern int yyparse();
extern void yyerror(char *);

#define COMMAND_INIT_SIZE 1024
int command_size = 0;
static char *command = NULL;

void append(const char *token);

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
                // sfree(&command);
                if(command) *command = '\0';    // 直接置首位为null，而非free释放
            }

            if (current_proc == NULL) {
                // debug("gen new proc...");
                current_proc = create_process();
            }

            process_add_arg(current_proc, strdup($1));
            append($1);

            free($1);
        }
;
arg: ID {   
            // debug("arg: %s", $1);
            process_add_arg(current_proc, strdup($1));
            append($1);

            free($1);
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
pipe: PIPE {append("|");}
;
simple_command_list: simple_command {}
    | simple_command_list pipe simple_command {}
;
redirection: LESS ID    {current_job->in_file = strdup($2);append("<");append($2);free($2);}
    | GREATER ID          {current_job->out_file = strdup($2);append(">");append($2);free($2);}
    | '2' GREATER ID      {current_job->err_file = strdup($3);append("2>");append($3);free($3);}
;
redirection_list: /* empty */
    | redirection_list redirection  {}
;
background: /* empty */
    | AMPERSAND {
        current_job->fg = 0;
        append("&");
    }
;
terminator: NEWLINE     {}
    | SIMECOLON         {}
;
pipeline: terminator    {}
    | simple_command_list redirection_list background terminator    {}
;
pipeline_list: pipeline {
                    current_job->command = strdup(command);
                    debug("command: %s", command);
                    debug("start execution");
                    int res _unused = execute_job(current_job);
                    debug("the result of execution is: %d", res);
                    current_job = NULL;
                }
    | pipeline_list pipeline    {}
;


%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    /* free_job(current_job);
    free_process(current_proc);
    current_job = NULL;
    current_proc = NULL;
    sfree(&command); */
}

/*
自动空格分割的可变长字符串
*/
void append(const char *token) {
    if(!token) return;
    char *new_command;
    int token_len = strlen(token);
    if (token_len == 0) return;

    if (!command) { // 当前串为空时，申请max(固定配额，token串长度 + '\0')
        command_size = max(token_len + 1, COMMAND_INIT_SIZE);
        command = (char *)malloc(command_size);
        if (!command) panic("memory alloc failure");
        strcpy(command, token); // 拷贝
        return;
    }

    int old_len = strlen(command);  // 获取旧串长度
    int new_len = old_len + token_len + 2;  //+ 1 '\0' + 1空格分割
    if (new_len > command_size) {   // 超出max_size
        while (command_size < new_len) command_size *= 2; // 不断×2扩容直到满足大小
        new_command = (char *)malloc(command_size);
        if (!new_command) panic("memory alloc failure");
        strcpy(new_command, command);   // 拷贝旧串
        free(command);  // 释放旧串
        command = new_command;
    }
    *(command + old_len) = ' '; // 添加空格分割
    strcpy(command + old_len + 1, token); // 拷贝新串

}