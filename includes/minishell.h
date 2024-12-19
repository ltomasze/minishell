/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:40:34 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/19 17:30:32 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

// Funkcje parsujące
/*char    **tokenize_input(const char *input);*/

// Wbudowane polecenia
//void    cd_command(char *path);
void    pwd_command(void);

// Egzekucja poleceń
void    execute_command(char *command);

// Narzędzia pomocnicze
int     ft_strcmp(const char *s1, const char *s2);
//char    *ft_strchr(const char *s, int c);
void    print_error(const char *prefix, const char *msg);

#endif