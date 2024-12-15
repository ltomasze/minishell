#include "minishell.h"

static int count_words(const char *str)
{
    int count = 0;
    int in_word = 0;

    while (*str)
    {
        if (*str != ' ' && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (*str == ' ')
            in_word = 0;
        str++;
    }
    return count;
}

static char **split_input(const char *input)
{
    int     i = 0;
    char    *token;
    char    *copy = strdup(input);
    char    **words = malloc((count_words(input) + 1) * sizeof(char *));

    if (!words || !copy)
        return NULL;
    token = strtok(copy, " ");
    while (token)
    {
        words[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    words[i] = NULL;
    free(copy);
    return words;
}

t_cmd *parse_command(const char *input)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));

    if (!cmd)
        return NULL;
    char **words = split_input(input);
    if (!words)
    {
        free(cmd);
        return NULL;
    }
    cmd->command = strdup(words[0]);
    cmd->args = words;
    return cmd;
}

void free_command(t_cmd *cmd)
{
    int i = 0;

    if (!cmd)
        return;
    while (cmd->args[i])
        free(cmd->args[i++]);
    free(cmd->args);
    free(cmd->command);
    free(cmd);
}
