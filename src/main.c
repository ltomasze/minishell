/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:41:44 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/19 17:30:07 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

void shell_loop(void)
{
    char *command;

    while (1)
    {
        command = readline("msh> ");
        if (!command)  // Obsługa Ctrl-D (EOF)
        {
            write(1, "exit\n", 5);
            break;
        }

        if (*command)  // Dodajemy do historii tylko niepuste komendy
            add_history(command);

        execute_command(command);  // Egzekucja komendy

        if (ft_strcmp(command, "exit") == 0)  // Sprawdzanie komendy "exit"
        {
            free(command);  // Zwalnianie pamięci przed zakończeniem
            break;  // Zakończenie pętli
        }

        free(command);  // Zwalnianie pamięci po każdym poleceniu
    }
}

int main(void)
{
    shell_loop();
    return (0);
}
