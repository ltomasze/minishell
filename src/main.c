#include "minishell.h"

int main(void)
{
    char *input;
    t_cmd *cmd;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (*input)
        {
            add_history(input);
            cmd = parse_command(input);
            if (cmd)
            {
                execute_command(cmd);
                free_command(cmd);
            }
        }
        free(input);
    }
    return 0;
}
