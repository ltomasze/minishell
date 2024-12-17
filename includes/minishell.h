/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 14:52:08 by ltomasze          #+#    #+#             */
/*   Updated: 2024/12/17 16:18:23 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stddef.h>  // Dla NULL

char    *get_command(void);
void exit_msh(char *command);
void cd_command(char *path);
char *ft_strtok(char *str, const char *separ);
void execute_command(char *command);
char *ft_strchr(const char *s, int c);
int ft_strcmp(const char *s1, const char *s2);

#endif