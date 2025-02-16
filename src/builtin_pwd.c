/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:14:01 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:14:37 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
The `pwd_bltin` function implements the built-in `pwd` command,  
which prints the current working directory. It uses the `getcwd` function  
to retrieve the current path, and if an error occurs, it calls the `ft_perror_message`  
function and exits with an error code of `1`. If the path is successfully retrieved,  
it prints it to the standard output. The function exits with a status code of `0`.  
It is part of the *minishell* project, replicating the `pwd` command in the command interpreter.
*/
void	pwd_bltin(void)
{
	char	path[4096];

	if (!(getcwd(path, 4096)))
	{
		ft_perror_message();
		exit(1);
	}
	else
		printf("%s\n", path);
	exit(0);
}
