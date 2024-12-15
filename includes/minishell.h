#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

typedef struct s_cmd
{
    char    *command;  // Nazwa komendy
    char    **args;    // Lista argumentów
}   t_cmd;

// Prototypy funkcji
t_cmd   *parse_command(const char *input);
void    free_command(t_cmd *cmd);
void execute_command(t_cmd *cmd);

#endif
