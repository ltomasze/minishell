#include "minishell.h"


static int execute_builtin(t_cmd *cmd)
{
    if (strcmp(cmd->command, "echo") == 0)
    {
        int i = 1;
        int newline = 1;
        
        // Obsługa opcji -n w echo
        if (cmd->args[i] && strcmp(cmd->args[i], "-n") == 0)
        {
            newline = 0;
            i++;
        }

        for (; cmd->args[i]; i++)
        {
            printf("%s", cmd->args[i]);
            if (cmd->args[i + 1])
                printf(" ");
        }
        if (newline)
            printf("\n");

        return 1; // Komenda wbudowana
    }
    else if (strcmp(cmd->command, "cd") == 0)
    {
        if (cmd->args[1])
        {
            if (chdir(cmd->args[1]) != 0)
                perror("cd");
        }
        else
            chdir(getenv("HOME"));

        return 1; // Komenda wbudowana
    }
    else if (strcmp(cmd->command, "pwd") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("pwd");

        return 1; // Komenda wbudowana
    }

    return 0; // Komenda zewnętrzna
}

void execute_command(t_cmd *cmd)
{
    pid_t pid;
    int status;

    if (execute_builtin(cmd))
        return;

    // Wykonanie komendy zewnętrznej
    pid = fork();
    if (pid == 0)
    {
        if (execve(cmd->command, cmd->args, NULL) == -1)
        {
            perror(cmd->command);
            exit(1);
        }
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            printf("Exit status: %d\n", WEXITSTATUS(status));
    }
    else
    {
        perror("fork");
        exit(1);
    }
}
