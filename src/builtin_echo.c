/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltomasze <ltomasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:06:24 by ltomasze          #+#    #+#             */
/*   Updated: 2025/02/16 15:09:42 by ltomasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_echo_args(char **cmd);

/*
The echo_bltin function implements the behavior of the built-in echo command, 
printing the arguments passed in cmd to the standard output. It handles the -n flag, 
which skips appending a newline character at the end. The function processes each argument, 
checking the correctness of the -n flag. If the flag is correctly present, 
it sets the flag variable to true and omits adding a newline after printing the arguments. 
After processing, the function calls ft_echo_args to print the proper arguments and completes the process with exit code 0. 
This function replicates the behavior of the echo command in minishell.
*/
void	echo_bltin(char **cmd)
{
	int		i;
	int		j;
	bool	flag;

	i = 1;
	j = 0;
	flag = false;
	while (cmd[i])
	{
		if (!cmd[i][j] || cmd[i][j] != '-')
			break ;
		j++;
		while (cmd[i][j] && cmd[i][j] == 'n')
			j++;
		if (cmd[i][j] || j == 1)
			break ;
		j = 0;
		flag = true;
		i++;
	}
	ft_echo_args(&cmd[i]);
	if (flag == false)
		printf("\n");
	exit(0);
}

/*
The ft_echo_args function iterates over the arguments passed in the cmd array (containing commands 
and their arguments), printing each one to the standard output, adding a space between the arguments, 
but not at the end. After completing this, it ends the execution. 
This implementation in the project performs a similar function to the standard echo command, 
printing the command arguments to the screen without adding a newline by default. 
The function is used to display all arguments without any extra formatting.
*/
static void	ft_echo_args(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		i++;
		if (cmd[i])
			printf(" ");
	}
}
