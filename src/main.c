/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:51:36 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/17 16:31:43 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strchr(const char *s, int c)
{
    while (*s)  // Przechodzimy przez ciąg znaków aż do końca
    {
        if (*s == (char)c)  // Sprawdzamy, czy aktualny znak to ten, którego szukamy
            return (char *)s;  // Zwracamy wskaźnik na znaleziony znak
        s++;
    }

    // Sprawdzamy, czy znak 'c' to '\0' (koniec ciągu), wtedy zwracamy wskaźnik na null-terminator
    if (*s == (char)c)
        return (char *)s;

    return NULL;  // Zwracamy NULL, gdy nie znaleziono znaku
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))  // Przechodzimy przez ciągi, dopóki są równe
    {
        s1++;
        s2++;
    }

    // Zwracamy różnicę między pierwszymi różnymi znakami lub 0, jeśli ciągi są równe
    return (unsigned char)*s1 - (unsigned char)*s2;
}

char *get_command(void)
{
    char *command = readline("msh> ");
    if (command != NULL) 
        add_history(command);
    return (command);
}

void exit_msh(char *command)
{
    free(command);
    exit(0);
}

void cd_command(char *path)
{
    if (path == NULL)
        path = getenv("HOME");
    // Przechodzimy do zadanego katalogu
    if (chdir(path) != 0)
    {
        perror("cd");
    }
}

char *ft_strtok(char *str, const char *separ)
{
    static char *last = NULL;
    char *start;

    if (str == NULL && last == NULL)
        return NULL;
    if (str == NULL)
        str = last;
    while (*str && (ft_strchr(separ, *str) != NULL))
        str++;
    if (*str == '\0')
        return NULL;
    start = str;
    // Szukamy separatorów
    while (*str && (ft_strchr(separ, *str) == NULL))
        str++;
    if (*str)
        *str++ = '\0';
    last = str;
    return (start);
}

void execute_command(char *command)
{
    // Jeśli komenda to "cd", wydzielamy ścieżkę
    if (ft_strcmp(command, "cd") == 0)
    {
        char *path = ft_strtok(NULL, " \t");  // Spacja i tabulator jako separatory
        cd_command(path);
    }
    else if (ft_strcmp(command, "exit") == 0)
        exit_msh(command);
    else
        printf("command not found: %s\n", command);
}

int main(void)
{
    char *command;

    while (1) 
    {
        command = get_command();
        if (command != NULL) 
        {
            execute_command(command);
            free(command);
        }
    }
    return (0);
}